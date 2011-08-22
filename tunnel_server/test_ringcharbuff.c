#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

#include"ringcharbuff.h"


#define MAXSTR 1024

int chop(char *str_in) {
    int length;
    length=(int)strlen(str_in);
    if(str_in[length-1]!='\n') return(0);
    str_in[length-1]='\0';
    return(1);
}

int readline(FILE *fp,char *line_out,size_t nbytes) {
    int out;
    char *res;
    res = fgets(line_out,nbytes,fp);
    if(res==NULL) {
        return 0;
    }
    out = strlen(line_out);
    chop(line_out);
    return out;
}

int usage(char *prog) {
    printf("usage is <%s> ringsize\n",prog);
    printf("\n");
    printf("Play with the ringcharbuff\n");
    printf("Use commands\n");
    printf("   +<STR># to add string STR\n");
    printf("   r<n># to read nbytes\n");
    printf("   -<n># to remove nbytes\n");
    printf("   s<n># to resize ringbuff\n");
    printf("   l<STR> add STR as a line\n");
    return 0;
}


int main(int argc,char **argv) {
    FILE *fpi;
    ringcharbuff_t *r;
    int ringsize;
    int err;
    int w;
    char tmp_line[MAXSTR+1];
    char line[MAXSTR+1];
    char data[MAXSTR+1];
    char *l_ptr;
    int len;
    int is_eof;
    int resp;

    if(argc<2) {
        usage(argv[0]);
        return 0;
    }

    ringsize = atoi(argv[1]);

    printf("mypid = %i\n",getpid());
    r = ringcharbuff_init(ringsize,&err);
    ringcharbuff_print(r);
    while(1) {
        printf(">");
        fflush(stdout);
        resp = readline(stdin,line,MAXSTR);
        if(resp<=0){
            printf("rl resp= %i\n",resp);
            break;
        }
        switch(line[0]) {
            case 'l': l_ptr = &line[1];
                      strncat(l_ptr,"\n",MAXSTR);
                      len = strlen(l_ptr);
                      resp = ringcharbuff_add(r,l_ptr,len);
                      printf("Wrote %i bytes\n",resp);
                      break;

            case '+': l_ptr = &line[1];
                      len = strlen(l_ptr);
                      resp = ringcharbuff_add(r,l_ptr,len);
                      printf("Wrote %i bytes\n",resp);
                      break;

            case '-': l_ptr = &line[1];
                      len = atoi(l_ptr);
                      resp = ringcharbuff_dec(r,len);
                      printf("removed %i bytes\n",resp);
                      break;

            case 'g': l_ptr = &line[1];
                      len = atoi(l_ptr);
                      resp = ringcharbuff_get(r,data,len);
                      data[resp]='\0';
                      printf("Read %i bytes \"%s\"\n",resp,data);
                      break;

            case 'h': l_ptr = &line[1];
                      len = atoi(l_ptr);
                      resp = ringcharbuff_hasline(r);
                      printf("%i = hasline(r)\n",resp);
                      break;

            case 'i': printf("pid=%i\n",getpid());
                      break;

            case 'p': ringcharbuff_print(r);
                      break;
                      
            case 'r': l_ptr = &line[1];
                      len = atoi(l_ptr);
                      printf("%i\n",len);
                      resp = ringcharbuff_resize(r,len,&err);
                      if(!resp) {
                          printf("Error in resize %i\n",err);
                      } else {
                          printf("Resize success\n");
                      }
                      break;

            case 'u': printf("Used=%i\n",ringcharbuff_used(r));
                      break;

            case 'f': printf("free=%i\n",ringcharbuff_free(r));
                      break;

            case 's': printf("size=%i\n",ringcharbuff_size(r));
                      break;

            case 'q': case 'Q': exit(0);
                                break;

            default:  printf("Unknown Symbol use Q to quite\n");
                      break;
        }
    }
}
