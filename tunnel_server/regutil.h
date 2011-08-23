#include<pcre.h>

#ifndef __REGUTIL_H
#define __REGUTIL_H

typedef struct{
    pcre *p;
    int maxgroups;
}re_t;

typedef struct{
    re_t *re;
    char *line;
    int *ov;
    int ngroups;
}matcher_t;
#endif

char *getGroup(int idx,int *ov,char *strIn,int *error);
int showString(char *str);
