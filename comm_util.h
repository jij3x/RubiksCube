#ifndef COMM_UTIL_H_
#define COMM_UTIL_H_

#include "globe_def.h"

typedef struct dl_list_node {
    void *elem_ptr;
    struct dl_list_node *next, *prev;
} dl_list_node_t;

typedef void (*free_func_ptr_t)(void *);

typedef struct dl_list {
    const size_t elem_size;
    const free_func_ptr_t elemfree_func;
    dl_list_node_t ss_guard;
    dl_list_node_t *head, *tail, *iter;
    size_t count;
} dl_list_t;

dl_list_t *dl_list_create(size_t elem_size, free_func_ptr_t elemfree_func);
void dl_list_destroy(dl_list_t *list);
void dl_list_append(dl_list_t *list, void *element);
void dl_list_insert(dl_list_t *list, void *element);
void dl_list_remove(dl_list_t *list);
void dl_list_rm_first(dl_list_t *list);
void dl_list_rm_last(dl_list_t *list);
void dl_list_clear(dl_list_t *list);
void dl_list_rst_iter(dl_list_t *list);
dl_list_node_t *dl_list_get_next(dl_list_t *list);

#define HTBL_INIT_SIZE 128

typedef struct h_tbl_bucket {
    void *key;
    uint64_t hash_code;
    dl_list_node_t *entry;
} h_tbl_bucket_t;

typedef bool (*equals_func_ptr_t)(void *, void *);

typedef struct h_tbl {
    size_t key_count;
    size_t size;
    h_tbl_bucket_t *buckets, *val_iter;
    equals_func_ptr_t equals_func;
} h_tbl_t;

h_tbl_t *h_tbl_create(equals_func_ptr_t equals_func);
void h_tbl_destroy(h_tbl_t *tbl);
void *h_tbl_get(h_tbl_t *tbl, uint64_t key);
void h_tbl_add(h_tbl_t *tbl, uint64_t key, void *value);
void h_tbl_remove(h_tbl_t *tbl, uint64_t key);
void h_tbl_clear(h_tbl_t *tbl);
void h_tbl_rehash(h_tbl_t *tbl, size_t size);
void h_tbl_rst_iter(h_tbl_t *tbl);
void *h_tbl_get_nextval(h_tbl_t *tbl);

uint64_t djb2_hash(unsigned char *str);

#endif /* COMM_UTIL_H_ */
