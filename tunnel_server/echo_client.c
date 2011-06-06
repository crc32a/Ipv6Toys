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
#define BLOCKSIZE 4096
#define INITRINGSIZE 16738

int usage(char *prog) {
    printf("Usage is %s <host> <port> <ipv6|ipv4|any>\n",prog);
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

int echo_client(int cs){
    char block[BLOCKSIZE];
    int err;
    ssize_t bytesr;
    ssize_t bytesw;
    int stat;
    int si;
    int so;
    int eof;
    size_t size;
    fd_set rs,ws;
    ringcharbuff_t *sring;
    ringcharbuff_t *cring;
    sring = ringcharbuff_init(INITRINGSIZE,&err);
    cring = ringcharbuff_init(INITRINGSIZE,&err);

    si = fileno(stdin);
    so = fileno(stdout);
    setnonblocking(cs);
    setnonblocking(si);
    setnonblocking(so);

    while(1){
        FD_ZERO(&rs);
        FD_ZERO(&ws);
        FD_SET(si,&rs);
        FD_SET(cs,&rs);
        if(ringcharbuff_used(cring)>0) {
            FD_SET(so,&ws);
        }
        if(ringcharbuff_used(sring)>0) {
            FD_SET(cs,&ws);
        }
        stat = select(cs+1,&rs,&ws,NULL,NULL);
        if(stat<0){
            sleep(1);
            continue;
        }

        if(FD_ISSET(cs,&rs)){
            bytesr = myread(cs,block,BLOCKSIZE,&eof);
            if(eof){
                printf("EOF on fd %i\n",cs);
                return -1;
            }
            if(bytesr>= ringcharbuff_used(cring)){
                size = ringcharbuff_size(cring);
                size *= 2;
                ringcharbuff_resize(cring,size,&err);
            }
            ringcharbuff_add(cring,block,bytesr);
        }


        if(FD_ISSET(si,&rs)){
            bytesr = myread(si,block,BLOCKSIZE,&eof);
            if(eof){
                printf("EOF on fd %i\n",si);
                return -1;
            }
            if(bytesr>= ringcharbuff_used(sring)){
                size = ringcharbuff_size(sring);
                size *= 2;
                ringcharbuff_resize(sring,size,&err);
            }
            ringcharbuff_add(sring,block,bytesr);
        }

        if(FD_ISSET(so,&ws)){
            bytesr = ringcharbuff_get(cring,block,BLOCKSIZE);
            bytesw = mywrite(so,block,bytesr,&eof);
            if(eof){
                printf("EOF of stdout\n");
                return 0;
            }
            ringcharbuff_dec(cring,bytesw);
        }

        if(FD_ISSET(cs,&ws)){
            bytesr = ringcharbuff_get(sring,block,BLOCKSIZE);
            bytesw = mywrite(cs,block,bytesr,&eof);
            if(eof){
                printf("EOF of stdout\n");
                return 0;
            }
            ringcharbuff_dec(sring,bytesw);
        }
    }
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
    in_port_t uport;
    int rc;
    int cs;
    if(argc<3){
        usage(argv[0]);
        return 0;
    }

    host = argv[1];
    port = argv[2];
    ipv = argv[3];
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
    echo_client(cs);
    close(cs);
    return 0;
}


