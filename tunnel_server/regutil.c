#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int showString(char *str){
    int i;
    printf("%s\n",str);
    for(i=0;i<80;i++){
        if(i%10 == 0){
            printf("%i",i/10);
        }else{
            printf(" ");
        }
    }
    printf("\n");

    for(i=0;i<80;i++){
        printf("%i",i%10);
    }
    printf("\n");
}

char *getGroup(int idx,int *ov,char *strIn,int *error){
    int i;
    char *out;
    size_t size;
    char *oPtr;
    char *sPtr;
    int f;
    int l;
    f = ov[idx*2];
    l = ov[idx*2+1];
    size = l - f + 1;
    out = (char *)malloc(size);
    if(out == NULL){
        *error = 1;
        return NULL;
    }
    oPtr = out;
    sPtr = strIn + f;
    for(i=0;i<size-1;i++){
        *oPtr = *sPtr;
        oPtr++;
        sPtr++;
    }
    oPtr='\0';
    *error = 0;
    return out;
}

