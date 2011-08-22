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
#include<pcre.h>
#include"socktools.h"
#include"ringcharbuff.h"
#include"regutil.h"

#define STRSIZE 1024
#define BLOCKSIZE 4096
#define INITRINGSIZE 16738
#define SASIZE 64
#define BACKLOG 5


int usage(char *prog) {
    printf("usage is %s <lhost> <lport> <ipv> <msg>\n",prog);
    printf("\n");
    return 0;
}

int buildmessage(char *block,char *msg,size_t n){
    char *bodyfrm="<html><body><h1>%s</h1></body></html>\r\n";
    char *headersfrm="HTTP/1.1 200 OK\r\ncontent-type: text/html\r\ncontent-length: %i\r\n\r\n";
    char body[BLOCKSIZE + 1];
    char headers[BLOCKSIZE + 1];
    int cl;

    snprintf(body,BLOCKSIZE,bodyfrm,msg);
    cl = strlen(body);
    snprintf(headers,BLOCKSIZE,headersfrm,cl);

    snprintf(block,BLOCKSIZE,"%s%s",headers,body);
    return 0;
}

int clrstr(char *str,size_t size){
    int i;
    for(i=0;i<size;i++){
        str[i]='\0';
    }
    return 0;
}

int readuntil(int fd,char *end,size_t n){
    char ch;
    int i=0;
    ssize_t nbytes;
    while(i<n){
        nbytes=read(fd,&ch,sizeof(char));
        if(nbytes < 1){
            return 0;
        }
        if(ch == end[i]) {
            i++;
        }else{
            i = 0;
        }
    }
    printf("\n");
    return 0;
}


int hello_server(int cs,char *msg) {
    ssize_t nbytes;
    char block[BLOCKSIZE + 1];
    clrstr(block,BLOCKSIZE + 1);
    readuntil(cs,"\r\n\r\n",4);
    buildmessage(block,msg,BLOCKSIZE);
    nbytes = write(cs,block,strlen(block));
    printf("Wrote %i bytes\n",(int)nbytes);
    shutdown(cs,SHUT_WR);
    while(nbytes>0){
        nbytes = read(cs,block,BLOCKSIZE);
        printf("read %i bytes\n",(int)nbytes);
    }
    printf("Closing socket\n");
    return 0;
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
    char *msg;
    int ai_family;
    int ai_flags;
    int bs;
    int cs;
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


    if(argc<5){
        usage(argv[0]);
        return 0;
    }

    lhost = argv[1];
    lport = argv[2];
    lipv  = argv[3];
    msg   = argv[4];

    csa = sabuff;

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
                hello_server(cs,msg);
                close(cs);
                exit(0);
                break;
            default:
                close(cs);
                break;
        }
    }
    return 0;
}

