#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/time.h>
#include"socktools.h"
#include"ringcharbuff.h"

#define STRSIZE 1024
#define BLOCKSIZE 64*1024
#define INITRINGSIZE 64*1024
#define SLEEP 256000

#define CRLF "\r\n\r\n"

double gettimevalue()
   {
    double out;
    struct timeval tv;
    if(gettimeofday(&tv,NULL)==-1)
      {
       perror("\nError in call to gettimeofday(&tv)");
       return(-1.0);
      }
    out=(double)tv.tv_sec+(double)tv.tv_usec*0.000001;
    return(out);
   }

int usage(char *prog) {
    printf("Usage is %s <host> <port> <ipv6|ipv4|any> <req_str>\n",prog);
    printf("\n");
    printf("Connect to the remote echo server and\n");
    printf("send text\n");
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
            printf("EOF on socket %i\n",fd);
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

int lookup(char *host,char *port,int ai_family,struct addrinfo **result){
    int rc;
    struct addrinfo hints;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = ai_family;

    rc = getaddrinfo(host,port,&hints,result);
    return rc;
}

int echo_client(int cs,char *get_cmd){
    char block[BLOCKSIZE];
    char *reqstr;
    int err;
    ssize_t bytesr;
    ssize_t bytesw;
    int stat;
    int si;
    int so;
    int eof;
    size_t size;
    long tbytes = 0;
    long nbytes=0;
    double start_time;
    double end_time;
    double delta;
    reqstr = (char *)malloc(sizeof(char)*(strlen(get_cmd)+5));
    fd_set rs,ws;
    ringcharbuff_t *sring;
    ringcharbuff_t *cring;
    sring = ringcharbuff_init(INITRINGSIZE,&err);
    cring = ringcharbuff_init(INITRINGSIZE,&err);
    strcpy(reqstr,get_cmd);
    strcat(reqstr,CRLF);
    int is_eof;
    start_time = gettimevalue();
    printf("Wrote %li bytes in request \"%s\"\n",write(cs,reqstr,strlen(reqstr)),reqstr);
    while(1){
        nbytes = myread(cs,block,BLOCKSIZE,&is_eof);
        if(is_eof){
            printf("Got EOF exiting\n");
            break;
        }
        if(nbytes<0){
            break;
        }
        tbytes += nbytes;
    }
    end_time = gettimevalue();
    delta = end_time - start_time;
    printf("read a total of %li bytes in %f seconds\n",tbytes,delta);
    printf("read %f bytes per second\n",(double)tbytes/delta);
}

int sockconnect(struct addrinfo *res,char *host,in_port_t *uport,FILE *fp){
    struct addrinfo *cai;
    int s = -1;
    cai = res;
    while(cai != NULL){
        getipaddrstr(cai,host,uport,STRSIZE);
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
            return s;
        }
    }
    fprintf(stderr,"No more addrinfo to connect to\n");
    return -1;
}

int main(int argc, char **argv){
    struct addrinfo *res;
    struct addrinfo *caddr;
    char hname[STRSIZE + 1];
    char *ipv;
    int ai_family;
    char *host;
    char *port;
    char *get_cmd;
    in_port_t uport;
    int rc;
    int cs;
    if(argc<4){
        usage(argv[0]);
        return 0;
    }

    host = argv[1];
    port = argv[2];
    ipv = argv[3];
    get_cmd = argv[4];
    uport = atoi(port);

    ai_family = get_ai_family(ipv);
    rc = lookup(host,port,ai_family,&res);
        if(rc != 0) {
        fprintf(stderr,"Error getting addrinfo for %s:",host);
        fprintf(stderr,"%s\n",gai_strerror(rc));
        exit(1);
        }
    cs = sockconnect(res,host,&uport,stdout);
    freeaddrinfo(res);
    echo_client(cs,get_cmd);
    close(cs);
    return 0;
}


