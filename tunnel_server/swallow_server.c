#include<sys/socket.h>
#include<inttypes.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<stdio.h>
#include<errno.h>

#include"socktools.h"
#include"ringcharbuff.h"


#define STRSIZE 512
#define BUFFSIZE 4096
#define BACKLOG 5
#define SASIZE 64
#define ECHOSIZE 5

int usage(char *prog) {
    printf("usage is %s <localhostname> <localport>\n",prog);
    printf("\n");
    printf("Echo back the text the user inputs.\n");
    printf("\n");
    return 0;
}

int echo_server(int cs) {
    int64_t bytes_read=0;
    char *inptr;
    size_t nbytes;
    char outptr[BUFFSIZE + ECHOSIZE +1];

    inptr = outptr + ECHOSIZE;
    strcpy(outptr,"echo:");
    while(1) {
        nbytes = read(cs,inptr,BUFFSIZE);
        bytes_read += nbytes;
        if(nbytes==0) {
            break;
        }
    }
    printf("Read %" PRId64 " bytes from Client\n",bytes_read);
    close(cs);
    exit(0);
}


void burydead(){
    pid_t zombie = 1;
    while(1) {
        zombie = waitpid(-1,NULL,WNOHANG);
        if(zombie<1) {
            break;
        }
        printf("Killed pid %i\n",zombie);
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


int main(int argc,char **argv) {
    char *lport;
    char tempstr[STRSIZE+1];
    char hname[STRSIZE+1];
    char sname[STRSIZE+1];
    char *format;
    char *tempstrptr;
    char *lip;
    char errorstr[STRSIZE+1];
    char sabuff[SASIZE];
    char afstr[STRSIZE];
    int af;
    void *csa; // client sockaddr


    struct sigaction act;
    int error;
    struct addrinfo *la;  // local addrinfo to serv on
    struct addrinfo *ai;  // current addrinfo
    struct addrinfo hints; // hints
    int ss;  // server socket descripter
    int cs;  // incomming client socket
    int result;
    socklen_t alen;

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


    if(argc<3) {
        usage(argv[0]);
        exit(0);
    }

    lip = argv[1];
    lport = argv[2];

    csa = sabuff;

    memset(&hints,0,sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM ;
    hints.ai_protocol = IPPROTO_TCP;

    result = getaddrinfo(lip,lport,&hints,&la);
    if(result != 0) {
        fprintf(stderr,"Error getting addrinfo for %s:",lip);
        fprintf(stderr,"%s\n",gai_strerror(result));
        exit(1);
    }

    ss = bindsocket(la,&ai,stderr);
    if(ss == -1) {
        fprintf(stderr,"Error attempting to bind\n");
        exit(-1);
    }

    result = listen(ss,BACKLOG);
    if(result == -1) {
        error = errno;
        strerror_r(error,tempstr,STRSIZE);
        fprintf(stderr,"Error listening:%s\n",tempstr);
        exit(1);
    }

    while(1) {
        alen = SASIZE;
        cs = accept(ss,(struct sockaddr *)csa,&alen);
        if(cs == -1) {
            error = errno;
            strerror_r(error,tempstr,STRSIZE);
            fprintf(stderr,"Error accepting connection:%i:%s\n",error,tempstr);
            continue;
        }

        afstr[0]='\0';
        af = (uint16_t)((struct sockaddr *)csa)->sa_family;
        affamily2str(afstr,STRSIZE,af);

        result = getnameinfo((struct sockaddr*)csa,alen,hname,STRSIZE,
                             sname,STRSIZE,NI_NUMERICHOST|NI_NUMERICSERV);

        fprintf(stderr,"connection from:");

        if(result == 0){
            format = "(%i(%s) addr=\"%s\" port=\"%s\")\n";
            fprintf(stderr,format,af,afstr,hname,sname);
        } else {
            strncpy(errorstr,gai_strerror(result),STRSIZE);
            format = "Unknown because: af=%i(%s)%s\n";
            fprintf(stderr,format,af,afstr,errorstr);
        }

        switch(fork()) {
            case -1:
                fprintf(stderr,"Error forking\n");
                break;
            case 0:
                echo_server(cs);
            default:
                close(cs);
                break;
        }
    }
    return 0;
}
