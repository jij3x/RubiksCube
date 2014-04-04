#include "comm_util.h"

sll_t *sll_create(size_t elem_size, free_func_ptr_t elemfree_func) {
    sll_t init_list = { .elem_size = elem_size, .elemfree_func = elemfree_func };
    sll_t *list = malloc(sizeof(sll_t));
    if (list == NULL)
        abort();
    memcpy(list, &init_list, sizeof(sll_t));

    list->head = NULL;
    list->tail = NULL;
    list->iter = &list->ss_guard;
    list->count = 0;

    return list;
}

void sll_clear(sll_t *list) {
    while (list->head != NULL) {
        sll_node_t *node = list->head;
        list->head = list->head->next;

        if (list->elemfree_func != NULL)
            list->elemfree_func(node->elem_ptr);
        else
            free(node->elem_ptr);

        free(node);
    }

    list->tail = NULL;
    list->ss_guard.next = NULL;
    list->count = 0;
    list->iter = &list->ss_guard;
}

void sll_destroy(sll_t *list) {
    sll_clear(list);

    free(list);
//    list = NULL;
}

void sll_append(sll_t *list, void *element) {
    sll_node_t *node = malloc(sizeof(sll_node_t));
    node->elem_ptr = malloc(sizeof(list->elem_size));
    memcpy(node->elem_ptr, element, list->elem_size);
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        list->ss_guard.next = list->head;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->count++;
}

void sll_reset_iter(sll_t *list) {
    list->iter = &list->ss_guard;
}

sll_node_t *sll_get_next(sll_t *list) {
    sll_node_t *node = list->iter->next;

    if (list->iter->next != NULL)
        list->iter = list->iter->next;

    return node;
}

uint64_t djb2_hash(unsigned char *str) {
    uint64_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


