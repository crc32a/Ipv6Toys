#include<stdlib.h>
#include<stdio.h>
#include"ringcharbuff.h"

ringcharbuff_t *ringcharbuff_init(size_t size,int *err) {
    char *data;
    ringcharbuff_t *r;
    data = (char *)malloc(sizeof(char)*size);
    if(data==NULL) {
        *err=1;
        return NULL;
    }
    r = (ringcharbuff_t *)malloc(sizeof(ringcharbuff_t));
    if(r==NULL) {
        free(data);
        *err=2;
        return NULL;
    }
    r->size =size;
    r->h = 0;
    r->used = 0;
    r->data = data;
    *err=0;
    return r;
}

int ringcharbuff_used(ringcharbuff_t *r) {
    return r->used;
}

int ringcharbuff_size(ringcharbuff_t *r) {
    return r->size;
}

int ringcharbuff_free(ringcharbuff_t *r) {
    return r->size - r->used;
}

int ringcharbuff_print(ringcharbuff_t *r) {
    int i;
    int h = r->h;
    int s = ringcharbuff_size(r);
    int u = ringcharbuff_used(r);
    int f = ringcharbuff_free(r);

    printf("    ");
    for(i=0;i<s;i++) {
        if(i==h) {
            printf("%s","h");
        }
        else {
            printf("%s"," ");
        }
    }
    printf("\n    ");

    for(i=0;i<s;i++) {
        if(i%10==0) {
            printf("%i",i/10);
        }
        else {
            printf(" ");
        }
    }
    printf("\n    ");

    for(i=0;i<s;i++) {
        printf("%i",i%10);
    }
    printf("\n    ");

    for(i=0;i<s;i++) {
        printf("%c",r->data[i]);
    }
    printf("\n    ");
    printf("u=%i f=%i s=%i\n",u,f,s);
    return 0;
}

int ringcharbuff_get(ringcharbuff_t *r,char *data,size_t nbytes) {
    int u = r->used;
    int di = 0;
    int ri = r->h;
    if(r->size == 0) return 0;
    while( di < nbytes && u > 0) {
        data[di] = r->data[ri];
        ri = (ri+1)%r->size;
        di++;
        u--;
    }
    return di;
}

int ringcharbuff_add(ringcharbuff_t *r,char *data,size_t nbytes) {
    int f  = ringcharbuff_free(r);
    int di = 0; // index for parameter data
    int ri;
    if(r->size == 0) return 0;
    ri = (r->h+r->used)%r->size;
    while(di < nbytes && f>0) {
        r->data[ri] = data[di];
        ri = (ri+1)%r->size;
        di++;
        f--;
    }
    r->used += di;
    return di;
}

int ringcharbuff_dec(ringcharbuff_t *r,size_t nbytes) {
    int n = (nbytes < r->used) ? nbytes : r->used;
    if(r->size == 0) return 0;
    r->h = (r->h + n)%r->size;
    r->used -= n;
    return n;   
}

int ringcharbuff_clear(ringcharbuff_t *r){
    return ringcharbuff_dec(r,r->used);
}

int ringcharbuff_empty(ringcharbuff_t *r){
    return r->used == 0;
}

int ringcharbuff_resize(ringcharbuff_t *r,size_t new_size,int *err) {
    int ri;
    int i;
    int used;
    int size;
    char *ndata;
    char *data;
    used = r->used;
    data = r->data;
    size = r->size; 
    if(new_size < used) {
        *err = 1;
        return 0;
    }
    ndata = (char *)malloc(sizeof(char)*new_size);
    if(ndata == NULL) {
        *err = 2;
        return 0;
    }

    if(size == 0) {
        r->h = 0;
        r->size = new_size;
        free(r->data);
        r->data = ndata;
        err = 0;
        return 1;
    }

    ri = r->h;
    for(i=0;i < used;i++) {
        ndata[i] = data[ri];
        ri = (ri+1)%size;
    }

    free(data);
    r->data = ndata;
    r->h = 0;
    r->size=new_size;
    *err=0;
    return 1;
}
