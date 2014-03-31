#ifndef COMM_UTIL_H_
#define COMM_UTIL_H_

#include "globe_def.h"

typedef void (*free_func_t)(void *);

typedef struct sll_node {
    void *elem_ptr;
    struct sll_node *next;
} sll_node_t;

typedef struct sll {
    const size_t elem_size;
    const free_func_t elemfree_func;
    sll_node_t ss_guard;
    sll_node_t *head, *tail, *iter;
    size_t count;
} sll_t;

sll_t *sll_create(size_t elem_size, free_func_t elemfree_func);
void sll_destroy(sll_t *list);
void sll_append(sll_t *list, void *element);
void sll_reset_iter(sll_t *list);
sll_node_t *sll_get_next(sll_t *list);

uint64_t djb2_hash(unsigned char *str);

#endif /* COMM_UTIL_H_ */
