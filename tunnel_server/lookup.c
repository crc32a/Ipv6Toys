#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>

#include"socktools.h"

#define STRSIZE 1024

int usage(char *prog) {
    printf("Usage is %s <remotehost> <ipv6|ipv4|any>\n",prog);
    printf("\n");
    printf("Look up the remote host\n");
    return 0;
}

int lookup(char *host,int ai_family,struct addrinfo **result){
    int rc;
    struct addrinfo hints;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = ai_family;

    rc = getaddrinfo(host,NULL,&hints,result);
    return rc;
}

int main(int argc,char **argv) {
    char *rhost;
    int rc;
    int ai_family;
    struct addrinfo *res;
    struct addrinfo *caddr;
    char hname[STRSIZE + 1];
    in_port_t port;
    if(argc<3){
        usage(argv[0]);
        return 0;
    }

    rhost = argv[1];
    printf("Looking up %s \n",rhost);

    ai_family = get_ai_family(argv[2]);

    rc = lookup(rhost,ai_family,&res);
    if(rc != 0) {
        fprintf(stderr,"Error getting addrinfo for %s:",rhost);
        fprintf(stderr,"%s\n",gai_strerror(rc));
        exit(1);
    }

    caddr = res;
    while(caddr != NULL){
        getipaddrstr(caddr,hname,&port,STRSIZE);
        printaddrinfo(caddr,hname,NULL,NULL,stdout);
        caddr = caddr->ai_next;
    }

    freeaddrinfo(res);
    return 0;
}

