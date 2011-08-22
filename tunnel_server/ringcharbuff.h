#ifndef __RINGFIFO_H
#define __RINGFIFO_H
typedef struct {
    int h;
    int size;
    int used;
    char *data;
}ringcharbuff_t;

#endif

ringcharbuff_t *ringcharbuff_init(size_t size,int *err);
int ringcharbuff_empty(ringcharbuff_t *r);
int ringcharbuff_print(ringcharbuff_t *r);
int ringcharbuff_used(ringcharbuff_t *r);
int ringcharbuff_size(ringcharbuff_t *r);
int ringcharbuff_free(ringcharbuff_t *r);
int ringcharbuff_add(ringcharbuff_t *r,char *data,size_t nbytes);
int ringcharbuff_get(ringcharbuff_t *r,char *data,size_t nbytes);
int ringcharbuff_dec(ringcharbuff_t *r,size_t nbytes);
int ringcharbuff_resize(ringcharbuff_t *r,size_t nbytes,int *err);
int ringcharbuff_clear(ringcharbuff_t *r);
int ringcharbuff_hasline(ringcharbuff_t *r);
