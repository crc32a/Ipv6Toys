#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

int socktype2str(char *buff,size_t buffsize,int st);
int protocol2str(char *buff,size_t buffsize,int pf);
int affamily2str(char *buff,size_t buffsizee, int af);
int printaddrinfo(struct addrinfo *ai,char *hname,char *sname,in_port_t *port,FILE *fp);
int getipaddrstr(struct addrinfo *ai,char *hname,in_port_t *port,socklen_t buffsize);
int get_ai_family(char *ipv); // "ipv6"=AF_INET6 "ipv4"=AF_INET else AF_UNSPEC
int get_ai_socktype(char *socktype); // "tcp"=SOCK_STREAM udp="SOCK_DGRAM"
int strnlower(char *dst,char *src,size_t n);
