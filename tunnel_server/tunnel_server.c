#include<sys/socket.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<netdb.h>

#include"socktools.h"
#include"ringcharbuff.h"

#define STRSIZE 1024
#define BLOCKSIZE 4096*128
#define INITRINGSIZE 16738
#define SASIZE 64
#define BACKLOG 5


int usage(char *prog) {
    printf("usage is %s <lhost> <lport> <ipv> <rhost> <rport> <ipv>\n",prog);
    printf("\n");
    return 0;
}

ssize_t myread(int fd,char *block,size_t blocksize,int *eof){
    ssize_t nbytes;
    *eof = 0;
    nbytes = read(fd,block,blocksize);
    if(nbytes == -1){
        if(errno == EAGAIN || errno == EWOULDBLOCK) {
            nbytes = 0;
            return nbytes;
        }else{
            printf("EOF on socket %i errno=%i\n",fd,errno);
            *eof = 1;
            nbytes;
            return nbytes;
        }
    }
    if(nbytes == 0){
        *eof=1;
        return nbytes;
    }
    return nbytes;
}

ssize_t mywrite(int fd,char *block,size_t blocksize,int *eof){
    ssize_t nbytes;
    *eof = 0;
    nbytes = write(fd,block,blocksize);
    if(nbytes == -1){
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            return 0;
        }else{
            *eof = 1;
            return 0;
        }
    }
    return nbytes;
}


int setnonblocking(int s){
    int flags;
    flags = fcntl(s,F_GETFL,0);
    fcntl(s,F_SETFL, flags | O_NONBLOCK);
}

int max(int a,int b) {
    if(a>b) return a;
    return b;
}

int ringup(ringcharbuff_t *r,int required) {
    int err;
    int size;
    while(ringcharbuff_free(r) < required){
        size = ringcharbuff_size(r);
        size *= 2;
        printf("resizeing ringchar to %i bytes\n",size);
        if(!ringcharbuff_resize(r,size,&err)){
            printf("Out of memory\n");
            exit(-1);
        }
    }
}

int fd_setStr(fd_set *fd,int cs,int ss,char *block,size_t bsize){
       int out;
       const char *format = "[%s,%s]";
       char *csStr = (FD_ISSET(cs,fd))?"cs":"";
       char *ssStr = (FD_ISSET(ss,fd))?"ss":"";
       out = snprintf(block,bsize,format,csStr,ssStr);
       return out;
    }

int fd_setsStr(fd_set *rs,fd_set *ws,int cs,int ss,char *block,size_t bsize){
    const char *format = "rs=%s,ws=%s";
    char rsStr[STRSIZE +1 ];
    char wsStr[STRSIZE +1 ];
    fd_setStr(rs,cs,ss,rsStr,STRSIZE);
    fd_setStr(ws,cs,ss,wsStr,STRSIZE);
    snprintf(block,bsize,format,rsStr,wsStr);
}

int FD_ISEMPTY(fd_set *fs,int cs,int ss) {
    return (!FD_ISSET(cs,fs)  && !FD_ISSET(ss,fs));
}

int proxy_server(int cs,int ss) {
    pid_t pid;
    int rfree;
    int stat;
    int err;
    char block[BLOCKSIZE + 1];
    char tstr[STRSIZE + 1];
    ringcharbuff_t *cr;
    ringcharbuff_t *sr;
    cr = ringcharbuff_init(INITRINGSIZE,&err);
    sr = ringcharbuff_init(INITRINGSIZE,&err);
    setnonblocking(cs);
    setnonblocking(ss);
    ssize_t br;
    ssize_t bw;
    fd_set rs;
    fd_set ws;
    int nfds;
    int ceof[] = {0,0};
    int seof[] = {0,0};
    nfds = max(cs,ss) + 1;
    pid = getpid();
    while(!ceof[0] || !ceof[1] || !seof[0] || !seof[1]){
        FD_ZERO(&rs);
        FD_ZERO(&ws);

        printf("pid[%i] ceof = {%i,%i} seof={%i,%i} ",pid,ceof[0],ceof[1],seof[0],seof[1]);
        printf("cr=(%i,%i) sr=(%i,%i)\n",ringcharbuff_used(cr),ringcharbuff_size(cr),ringcharbuff_used(sr),ringcharbuff_size(sr));
        fflush(stdout);

        if(!ceof[0]) FD_SET(cs,&rs);
        if(!seof[0]) FD_SET(ss,&rs);

        if(seof[1]) ringcharbuff_clear(cr);
        if(ceof[1]) ringcharbuff_clear(sr);

        if(!seof[1] && !ringcharbuff_empty(cr)) {
            printf("pid[%5i] needs to write %i bytes to cr\n",
                 pid,ringcharbuff_used(cr));
            FD_SET(ss,&ws);
        }
        if(!ceof[1] && !ringcharbuff_empty(sr)) {
            printf("pid[%5i] needs to write %i bytes to ss\n",
                 pid,ringcharbuff_used(cr));
            FD_SET(cs,&ws);
        }

        if(ceof[0] && ringcharbuff_empty(cr) && !seof[1]){
            printf("cs.read closed and nothing left to write closeing ss.write\n");
            shutdown(ss,SHUT_WR);
            seof[1] = 1;
        }

        if(seof[0] && ringcharbuff_empty(sr) && !ceof[1]){
            printf("ss.read closed and nothing left to write closeing cs.write\n");
            shutdown(cs,SHUT_WR);
            ceof[1] = 1;
        }


        fd_setsStr(&rs,&ws,cs,ss,tstr,STRSIZE);
        printf("select(%s) = ",tstr);
        fflush(stdout);
        if(FD_ISEMPTY(&rs,cs,ss) && FD_ISEMPTY(&ws,cs,ss)){
            printf("rs and ws are both empty bailing exiting\n");
            break;
        }
        stat = select(nfds,&rs,&ws,NULL,NULL);
        fd_setsStr(&rs,&ws,cs,ss,tstr,STRSIZE);
        printf("%s\n",tstr);
        if(stat < 0){
            sleep(1);
            continue;
        }



        if(FD_ISSET(cs,&rs)) {
            br = myread(cs,block,BLOCKSIZE,&stat);
            rfree = ringcharbuff_free(cr);
            printf("pid[%5i] read %i bytes from cs\n",pid,(int)br);
            if(br > rfree) {
                ringup(cr,br);
            }
            ringcharbuff_add(cr,block,br);
            if(stat) {
                ceof[0] = 1;
                printf("pid[%5i] eof found cs.read\n",pid);
            }
        }

        if(FD_ISSET(ss,&rs)) {
            stat = 0;
            br = myread(ss,block,BLOCKSIZE,&stat);
            rfree = ringcharbuff_free(sr);
            printf("pid[%5i] read %i bytes from ss\n",pid,(int)br);
            if(br > rfree) {
                ringup(sr,br);
            }
            ringcharbuff_add(sr,block,br);
            if(stat) {
                seof[0] = 1;
                printf("pid[%5i] eof found ss.read\n",pid);
            }
        }

        if(FD_ISSET(ss,&ws)) {
            br = ringcharbuff_get(cr,block,BLOCKSIZE);
            bw = mywrite(ss,block,br,&stat);
            ringcharbuff_dec(cr,bw);
            printf("pid[%5i] write %i bytes to ss\n",pid,(int)bw);
            if(stat){
                seof[1] = 1;
                ceof[0] = 1;
                shutdown(cs,SHUT_RD);
                shutdown(ss,SHUT_WR);
                printf("pid[%5i] eof found ss.write\n",pid);
                printf("pid[%5i] set seof[1] = 1\n",pid);
                printf("pid[%5i] set ceof[0] = 1\n",pid);
                printf("pid[%5i] shutdown(cs,SHUT_RD);",pid);
                printf("pid[%5i] shutdown(ss,SHUT_WR);",pid);
            }
            if(ringcharbuff_empty(cr) && ceof[0]){
                seof[1] = 1;
                shutdown(ss,SHUT_WR);
                printf("pid[%5i] ringcharbuff_empty(cr) && ceof[0]\n",pid);
                printf("pid[%5i] seof[1] = 1 shutdown(ss,SHUT_WR)\n",pid);
            }
        }


        if(FD_ISSET(cs,&ws)) {
            br = ringcharbuff_get(sr,block,BLOCKSIZE);
            bw = mywrite(cs,block,br,&stat);
            ringcharbuff_dec(sr,bw);
            printf("pid[%5i] write %i bytes to cs\n",pid,(int)bw);
            if(stat){
                ceof[1] = 1;
                seof[0] = 1;
                shutdown(ss,SHUT_RD);
                shutdown(cs,SHUT_WR);
                printf("pid[%5i] eof found cs.write\n",pid);
                printf("pid[%5i] set ceof[1] = 1\n",pid);
                printf("pid[%5i] set seof[0] = 1\n",pid);
                printf("pid[%5i] shutdown(ss,SHUT_RD);",pid);
                printf("pid[%5i] shutdown(cs,SHUT_WR);",pid);
            }
            if(ringcharbuff_empty(sr) && seof[0]){
                ceof[1] = 1;
                shutdown(cs,SHUT_WR);
                printf("pid[%5i] ringcharbuff_empty(cr) && seof[0]\n",pid);
                printf("pid[%5i] seof[1] = 1 shutdown(ss,SHUT_WR)\n",pid);
            }
        }
    }
    return 0;
}

int sockconnect(struct addrinfo *res,struct addrinfo **ca,
    char *host,in_port_t *uport, socklen_t buffsize,FILE *fp){
    struct addrinfo *cai;
    int s = -1;
    cai = res;
    while(cai != NULL){
        getipaddrstr(cai,host,uport,buffsize);
        printf("Connect \n");
        printaddrinfo(cai,host,NULL,NULL,fp);
        s = socket(cai->ai_family,cai->ai_socktype,cai->ai_protocol);
        if(s == -1){
            perror("socket create failed");
            return -1;
        }
        if(connect(s,cai->ai_addr,cai->ai_addrlen)==-1) {
            perror("Connect Fail");
            close(s);
            s = -1;
            cai = cai -> ai_next;
            continue;
        }else{
            *ca = cai;
            return s;
        }
    }
    fprintf(stderr,"No more addrinfo to connect to\n");
    return -1;
}

int lookup(char *host,char *port,int ai_flags,int ai_family,struct addrinfo **result){
    int rc;
    struct addrinfo hints;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_flags = ai_flags;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = ai_family;

    rc = getaddrinfo(host,port,&hints,result);
    return rc;
}

void burydead(){
    pid_t zombie = 1;
    while(1) {
        zombie = waitpid(-1,NULL,WNOHANG);
        if(zombie<1) {
            break;
        }
        printf("buried zombie pid %i\n",zombie);
    }
    return;
}

int bindsocket(struct addrinfo *la,struct addrinfo **aiout,FILE *fout) {
    struct addrinfo *ai;
    char addrstr[STRSIZE+1];
    char errorstr[STRSIZE+1];
    ai = la; // current addrinfo
    int sd;
    int br;
    char *format;
    socklen_t slen;
    in_port_t port;


    sd = -1;
    for(ai=la; ai!= NULL;ai = ai->ai_next) {
        sd = socket(ai->ai_family,ai->ai_socktype,ai->ai_protocol);
        if(sd == -1) continue;
        slen = (socklen_t)ai->ai_addrlen;
        br = bind(sd,ai->ai_addr,slen);
        if(br == 0) {
            *aiout = ai;
            addrstr[0] = '\0';
            getipaddrstr(ai,addrstr,&port,STRSIZE);
            if(fout != NULL) {
                fprintf(fout,"Bound to interface %s\n",addrstr);
            }
            return sd;
        }else{
            addrstr[0] = '\0';
            getipaddrstr(ai,addrstr,&port,STRSIZE);
            if(fout != NULL){
                errorstr[0] = '\0';
                strerror_r(errno,errorstr,STRSIZE);
                fprintf(fout,"Coulden't bind to ");
                printaddrinfo(ai,addrstr,NULL,&port,fout);
                fprintf(fout,":%s\n",errorstr);
            }
        }
        close(sd);
        sd = -1;
    }
    if(fout != NULL) {
        fprintf(stderr,"No more interfaces to bind to\n");
    }
    return sd;
}



int main(int argc,char **argv){
    char *lhost;
    char *lport;
    char  *lipv;
    char  *rhost;
    char  *rport;
    char  *ripv;
    int ai_family;
    int ai_flags;
    int bs;
    int cs;
    int ss;
    int err;
    int res;
    int af;
    struct addrinfo *la;
    struct addrinfo *ra;
    struct addrinfo *ca;
    struct sigaction act;
    char tstr[STRSIZE + 1];
    char sabuff[SASIZE + 1];
    char sname[STRSIZE + 1];
    char hname[STRSIZE + 1];
    char afstr[STRSIZE +1];
    char errorstr[STRSIZE +1];
    char *format;
    void *csa; // Client sockaddr
    in_port_t uport;
    socklen_t alen;


    if(argc<7){
        usage(argv[0]);
        return 0;
    }

    lhost = argv[1];
    lport = argv[2];
    lipv  = argv[3];
    rhost = argv[4];
    rport = argv[5];
    ripv  = argv[6];

    csa = sabuff;

    printf("Starting tunnel server\n");
    printf("=======================================\n");
    printf("lhost = %s\n",lhost);
    printf("lport = %s\n",lport);
    printf("lipv = %s\n",lipv);
    printf("rhost = %s\n",rhost);
    printf("rport = %s\n",rport);
    printf("ripv = %s\n",ripv);
    printf("\n");

    printf("Registering zombie child collecter ...");
    fflush(stdout);
    act.sa_handler=burydead;
    if(sigemptyset(&act.sa_mask)==-1) {
        perror("\nError callin sigemptyset: ");
        exit(1);
    }
    act.sa_flags=0;
    if(sigaction(SIGCHLD,&act,NULL)==-1) {
       perror("\nCould not install SIGCHLD handler: ");
       exit(1);
    }
    printf("Registered\n");


    ai_family = get_ai_family(lipv);
    ai_flags = AI_PASSIVE;
    affamily2str(tstr,STRSIZE,ai_family);
    printf("Looking up %s %s %s for server port ",lhost,lport,lipv);
    printf("via ai_family = %s ... ",tstr);
    fflush(stdout);
    res = lookup(lhost,lport,ai_flags,ai_family,&la);
    if(res != 0){
        printf("error %s\n",gai_strerror(res));
        return 0;
    }

    printf("lookup worked\n");
    ca = la;
    while(ca != NULL){
        strcpy(tstr,"");
        getipaddrstr(ca,tstr,&uport,STRSIZE);
        printaddrinfo(ca,tstr,NULL,&uport,stdout);
        ca = ca -> ai_next;
    }

    printf("Attempting to bind ... ");
    fflush(stdout);

    bs = bindsocket(la,&ca,stdout);
    if(bs == -1){
        printf("bind Failed\n");
        exit(-1);
    }else{
        printf(" Useing ");
        strcpy(tstr,"");
        getipaddrstr(ca,tstr,&uport,STRSIZE);
        printaddrinfo(ca,tstr,NULL,&uport,stdout);
        printf("\n");
    }

    ai_family = get_ai_family(ripv);
    ai_flags = 0;
    affamily2str(tstr,STRSIZE,ai_family);
    printf("Looking up %s %s %s for remote server ",rhost,rport,ripv);
    printf("via ai_family = %s ... ",tstr);
    fflush(stdout);
    res = lookup(rhost,rport,ai_flags,ai_family,&ra);
    if(res != 0){
        printf("error %s\n",gai_strerror(res));
        return 0;
    }

    printf("lookup worked\n");
    ca = ra;
    while(ca != NULL){
        strcpy(tstr,"");
        getipaddrstr(ca,tstr,&uport,STRSIZE);
        printaddrinfo(ca,tstr,NULL,&uport,stdout);
        ca = ca -> ai_next;
    }

    printf("Attempting to connect to remote socket ... ");
    fflush(stdout);
    ss = sockconnect(ra,&ca,tstr,&uport,STRSIZE,stdout);
    if(ss == -1){
        printf("unable to connect server\n");
        exit(-1);
    }else{
        printf("Connected to ");
        strcpy(tstr,"");
        getipaddrstr(ca,tstr,&uport,STRSIZE);
        printaddrinfo(ca,tstr,NULL,&uport,stdout);
        printf("\n");
        close(ss);
    }

    printf("Listening ... ");
    res = listen(bs,BACKLOG);
    if(res == -1){
        strerror_r(errno,tstr,STRSIZE);
        printf("Failed: %s\n",tstr);
        exit(-1);
    }

    printf("Starting server loop\n");

    while(1){
        alen = SASIZE;
        cs = accept(bs,(struct sockaddr *)csa,&alen);
        if(cs == -1){
            strerror_r(errno,tstr,STRSIZE);
            printf("Client connect failed %s\n",tstr);
            continue;
        }


        afstr[0]='\0';
        af = (uint16_t)((struct sockaddr *)csa)->sa_family;
        affamily2str(afstr,STRSIZE,af);

        res = getnameinfo((struct sockaddr*)csa,alen,hname,STRSIZE,
                             sname,STRSIZE,NI_NUMERICHOST|NI_NUMERICSERV);

        fprintf(stderr,"connection from:");

        if(res == 0){
            format = "(%i(%s) addr=\"%s\" port=\"%s\")\n";
            fprintf(stderr,format,af,afstr,hname,sname);
        } else {
            strncpy(errorstr,gai_strerror(res),STRSIZE);
            format = "Unknown because: af=%i(%s)%s\n";
            fprintf(stderr,format,af,afstr,errorstr);
        }

        switch(fork()) {
            case -1:
                fprintf(stderr,"Error forking\n");
                break;
            case 0:
                printf("Attempting to connect to remote socket ... ");
                fflush(stdout);
                ss = sockconnect(ra,&ca,tstr,&uport,STRSIZE,stdout);
                if(ss == -1){
                    printf("unable to connect server\n");
                    exit(-1);
                }else{
                    printf("Connected to ");
                    strcpy(tstr,"");
                    getipaddrstr(ca,tstr,&uport,STRSIZE);
                    printaddrinfo(ca,tstr,NULL,&uport,stdout);
                    printf("\n");
                }
                proxy_server(cs,ss);
                close(cs);
                close(ss);
                exit(0);
                break;
            default:
                close(cs);
                break;
        }
    }
    return 0;
}

