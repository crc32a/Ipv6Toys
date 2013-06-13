#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"socktools.h"
#include"ringcharbuff.h"

#define STRSIZE 1024
#define BLOCKSIZE 4096

int usage(char *prog){
    printf("usage is %s <host> <port> <ipv6|ipv4|any> <dataSize>\n",prog);
    printf("\n");
    printf("Connect to the remote swallow server and send\n");
    printf("dataSize bytes for testing\n");
}

int swallow_client(int cs,int64_t data_size){
    char block[BLOCKSIZE];
    int64_t bytes_left = data_size;
    int i;
    for(i=0;i<BLOCKSIZE;i++){
        block[i]=(char)i%256;
    }

    while(bytes_left > 0){
        int bytes_write = (bytes_left>BLOCKSIZE)?BLOCKSIZE:bytes_left;
        write(cs,block,bytes_write);
        bytes_left -= bytes_write;
    }
    close(cs);
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



int main(int argc,char **argv){
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
    int64_t data_size;

    if(argc<4){
        usage(argv[0]);
        return 0;
    }
    host = argv[1];
    port = argv[2];
    ipv = argv[3];
    uport = atoi(port);
    data_size = (int64_t)atoll(argv[4]);
    ai_family = get_ai_family(ipv);
    rc = lookup(host,port,ai_family,&res);
        if(rc != 0) {
        fprintf(stderr,"Error getting addrinfo for %s:",host);
        fprintf(stderr,"%s\n",gai_strerror(rc));
        exit(1);
        }
    cs = sockconnect(res,host,&uport,stdout);
    freeaddrinfo(res);
    swallow_client(cs,data_size);
    close(cs);
    return 0;
}

