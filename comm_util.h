#ifndef COMM_UTIL_H_
#define COMM_UTIL_H_

#include "globe_def.h"

/*
 * Doubly Linked List section start
 */

typedef struct dl_list_node {
    void *elem_ptr;
    struct dl_list_node *next, *prev;
} dl_list_node_t;

typedef void (*free_fptr_t)(void *);

typedef struct dl_list {
    const size_t elem_size;
    const free_fptr_t free_func;
    dl_list_node_t ss_guard;
    dl_list_node_t *head, *tail, *iter;
    size_t count;
} dl_list_t;

dl_list_t *dl_list_create(size_t elem_size, free_fptr_t free_func);
void dl_list_destroy(dl_list_t *list);
void dl_list_append(dl_list_t *list, void *element);
void dl_list_insert(dl_list_t *list, void *element);
void dl_list_remove(dl_list_t *list);
void dl_list_rmfst(dl_list_t *list);
void dl_list_rmlst(dl_list_t *list);
void dl_list_clear(dl_list_t *list);
void dl_list_rstiter(dl_list_t *list);
dl_list_node_t *dl_list_get_next(dl_list_t *list);

/*
 * Perfect Hashing section start
 */

typedef struct sec_htbl {
    const uint32_t size, a, b;
    void * const * elements;
} sec_htbl_t;

typedef uint32_t (*keyhash_fptr_t)(void *);
typedef int (*keycmp_fptr_t)(void *, void *);

typedef struct perf_htbl {
    const uint32_t size, a, b;
    const sec_htbl_t *fst_slots;
    const keyhash_fptr_t keyhash_func;
    const keycmp_fptr_t keycmp_func;
    const free_fptr_t free_func;
} perf_htbl_t;

typedef struct kv_pair {
    void *key, *value;
    size_t vlen;
} kv_pair_t;

enum perf_htbl_create_rc_e {
    succeed, duplicate_keys, hash_key_collision, secondary_uvhash_collision
};

#define MAX_RETRIES 32

perf_htbl_t *perf_htbl_create(kv_pair_t kv_set[], size_t len, keyhash_fptr_t keyhash_func,
        keycmp_fptr_t keycmp_func, free_fptr_t free_func, int *rc);
void perf_htbl_destroy(perf_htbl_t *tbl);
void *perf_htbl_get(perf_htbl_t *tbl, void *key);
void perf_htbl_updtv(perf_htbl_t *tbl, void *key, void *val);

#endif /* COMM_UTIL_H_ */
