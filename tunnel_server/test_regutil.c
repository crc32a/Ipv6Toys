#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<pcre.h>
#include"regutil.h"

#define OV_SIZE 333
#define MAXSTR 4096

static int chop(char *str_in) {
    int length;
    length=(int)strlen(str_in);
    if(str_in[length-1]!='\n') return(0);
    str_in[length-1]='\0';
    return(1);
}

static int usage(char *prog){
    printf("Usage is %s <pattern_file>\n",prog);
    return 0;
}

int zero(char *str,int n){
    int i;
    for(i=0;i<n;i++){
        str='\0';
        str++;
    }
    return 0;
}

int test_re(char *patternStr,char *testStr,int options) {
    char *error;
    char *group;
    int offset;
    pcre *pattern_re;
    int ov[OV_SIZE];
    int rc;
    int i;
    int errno;

    showString(testStr);
    pattern_re = pcre_compile(patternStr,options,(const char **)&error,&offset,NULL);
    if(!pattern_re){
        printf("Error coulden't compile %s: %d,%s\n",
            patternStr,offset,error);
    }

    rc = pcre_exec(pattern_re,NULL,(const char *)testStr,strlen(testStr),0,0,ov,OV_SIZE);
    if(rc < 0){
        switch(rc){
            case PCRE_ERROR_NOMATCH:
                printf("No match found\n");
                break;
            default: 
                printf("Match error %i\n",rc);
        }
    }else{
        for(i=0;i<6;i++){
            printf("ov[%i]=%i\n",i,ov[i]);
        }
        for(i=0;i<rc;i++){
            group = getGroup(i,ov,testStr,&errno);
            printf("group(%i)=\"%s\"\n",i,group);
            free(group);
        }
    }
    return 0;
}

int main(int argc,char **argv){
    FILE *fpi;
    char *file_name;
    char patternStr[MAXSTR+1];
    char testStr[MAXSTR+1];
    int options=PCRE_CASELESS;
    char *result;

    if(argc<2){
        usage(argv[0]);
        exit(0);
    }
    file_name = argv[1];
    printf("reading file %s\n",file_name);
    fpi = fopen(file_name,"r");
    if(fpi==NULL){
        printf("Error opening %s\n",file_name);
        exit(0);
    }
    zero(patternStr,MAXSTR+1);
    zero(patternStr,MAXSTR+1);
    result = fgets(patternStr,MAXSTR,fpi);
    result = fgets(testStr,MAXSTR,fpi);
    chop(patternStr);
    fclose(fpi);
    printf("patternStr = \"%s\"\ntestStr = \"%s\"\n",patternStr,testStr);
    test_re(patternStr,testStr,options);
    return 0;
}

