#ifndef COMM_UTIL_H_
#define COMM_UTIL_H_

#include "globe_def.h"

/*
 * Singly Linked List section start
 */

typedef struct sl_list_node {
    void *elem_ptr;
    struct sl_list_node *next;
} sl_list_node_t;

typedef void (*free_fptr_t)(void *);

typedef struct sl_list {
    const uint32_t elem_size;
    const free_fptr_t free_func;
    sl_list_node_t ss_guard;
    sl_list_node_t *head, *tail, *iter;
    uint32_t count;
} sl_list_t;

sl_list_t *sl_list_create(uint32_t elem_size, free_fptr_t free_func);
void sl_list_destroy(sl_list_t *list);
void sl_list_append(sl_list_t *list, void *element);
void sl_list_insert(sl_list_t *list, void *element);
void sl_list_remove(sl_list_t *list);
void sl_list_rmfst(sl_list_t *list);
void sl_list_rmlst(sl_list_t *list);
void sl_list_clear(sl_list_t *list);
void sl_list_rstiter(sl_list_t *list);
sl_list_node_t *sl_list_get_next(sl_list_t *list);

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
    uint32_t vlen;
} kv_pair_t;

enum perf_htbl_create_rc_e {
    succeed, key_hash_collided, sec_uvhash_collided
};

#define MAX_RETRIES 32

perf_htbl_t *perf_htbl_create(kv_pair_t kv_set[], uint32_t len, keyhash_fptr_t keyhash_func,
        keycmp_fptr_t keycmp_func, free_fptr_t free_func, int *rc);
void perf_htbl_destroy(perf_htbl_t *tbl);
void *perf_htbl_get(perf_htbl_t *tbl, void *key);
void perf_htbl_updtv(perf_htbl_t *tbl, void *key, void *val);

#endif /* COMM_UTIL_H_ */
