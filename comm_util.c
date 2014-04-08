#include "comm_util.h"

dl_list_t *dl_list_create(uint32_t elem_size, free_fptr_t free_func) {
    dl_list_t init_list = { .elem_size = elem_size, .free_func = free_func };
    dl_list_t *list = malloc(sizeof(dl_list_t));
    if (list == NULL)
        abort();
    memcpy(list, &init_list, sizeof(dl_list_t));

    list->head = NULL;
    list->tail = NULL;
    list->iter = &list->ss_guard;
    list->count = 0;

    return list;
}

void dl_list_clear(dl_list_t *list) {
    while (list->head != NULL) {
        dl_list_node_t *node = list->head;
        list->head = list->head->next;

        if (list->free_func != NULL)
            list->free_func(node->elem_ptr);
        else
            free(node->elem_ptr);

        free(node);
    }

    list->tail = NULL;
    list->ss_guard.next = NULL;
    list->count = 0;
    list->iter = &list->ss_guard;
}

void dl_list_destroy(dl_list_t *list) {
    dl_list_clear(list);

    free(list);
    list = NULL;
}

void dl_list_append(dl_list_t *list, void *element) {
    dl_list_node_t *node = malloc(sizeof(dl_list_node_t));
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

void dl_list_reset_iter(dl_list_t *list) {
    list->iter = &list->ss_guard;
}

dl_list_node_t *dl_list_get_next(dl_list_t *list) {
    dl_list_node_t *node = list->iter->next;

    if (list->iter->next != NULL)
        list->iter = list->iter->next;

    return node;
}

uint32_t murmur3_32(const char *key, uint32_t len, uint32_t seed) {
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m = 5;
    static const uint32_t n = 0xe6546b64;

    uint32_t hash = seed;

    uint32_t* keydata = (uint32_t*) key; //used to extract 32 bits at a time
    int keydata_it = 0;

    while (len >= 4) {
        uint32_t k = keydata[keydata_it++];
        len -= 4;

        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2)) * m) + n;
    }

    const uint8_t * tail = (const uint8_t*) (keydata + keydata_it * 4);
    uint32_t k1 = 0;

    switch (len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
        //no break
    case 2:
        k1 ^= tail[1] << 8;
        //no break
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (32 - r1));
        k1 *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

const uint64_t PRIME_33B = ((uint64_t) 1 << 32) + 15;

uint32_t universal_hashing(uint32_t key, uint32_t slots, uint32_t a, uint32_t b) {
    return (((uint64_t) a * key + b) % PRIME_33B) % slots;
}

int uint32_cmpfunc(const void *a, const void *b) {
    return *(uint32_t *) a - *(uint32_t *) b;
}

void free_sec_htbl(void **tbl, uint32_t len, free_fptr_t free_func) {
    if (tbl == NULL)
        return;

    for (uint32_t i = 0; i < len; i++) {
        if (tbl[i] == NULL)
            continue;

        if (free_func == NULL) {
            free(tbl[i]);
            tbl[i] = NULL;
        } else {
            free_func(tbl[i]);
        }
    }
}

perf_htbl_t *perf_htbl_create(kv_pair_t kv_set[], uint32_t len, keyhash_fptr_t keyhash_func,
        keycmp_fptr_t keycmp_func, free_fptr_t free_func, int *rc) {
    assert(len > 0 && kv_set != NULL);

    srand(time(NULL));

    uint32_t hashcodes[len], ordered_hc[len];
    for (uint32_t i = 0; i < len; i++) {
        hashcodes[i] = keyhash_func(kv_set[i].key);
        ordered_hc[i] = hashcodes[i];
    }

    qsort(ordered_hc, len, sizeof(uint32_t), uint32_cmpfunc);
    for (uint32_t i = 1; i < len; i++) {
        if (ordered_hc[i - 1] == ordered_hc[i]) {
            *rc = key_hash_collided;
            return NULL;
        }
    }

    uint32_t a = ((rand() << 16) & (~RAND_MAX)) | rand();
    a = (a == 0 ? 1 : a);
    uint32_t b = ((rand() << 16) & (~RAND_MAX)) | rand();

    uint32_t count[len];
    for (uint32_t i = 0; i < len; i++) {
        count[i] = 0;
    }
    for (uint32_t i = 0; i < len; i++) {
        count[universal_hashing(hashcodes[i], len, a, b)]++;
    }

    sec_htbl_t *fst_slots = malloc(sizeof(sec_htbl_t) * len);
    if (fst_slots == NULL)
        abort();

    for (uint32_t i = 0; i < len; i++) {
        sec_htbl_t init_sec_htbl = { .size = 0, .elements = NULL };
        memcpy(&(fst_slots[i]), &init_sec_htbl, sizeof(sec_htbl_t));

        if (count[i] == 0) {
            continue;
        }

        for (size_t retries = 0; retries < MAX_RETRIES; retries++) {
            uint32_t size = pow(count[i], 2);
            void **init_elements = malloc(sizeof(void *) * size);
            if (init_elements == NULL)
                abort();

            for (uint32_t j = 0; j < size; j++) {
                init_elements[j] = NULL;
            }

            uint32_t aa = ((rand() << 16) & (~RAND_MAX)) | rand();
            aa = (aa == 0 ? 1 : aa);
            uint32_t bb = ((rand() << 16) & (~RAND_MAX)) | rand();

            uint32_t k = 0;
            for (; k < len; k++) {
                if (universal_hashing(hashcodes[k], len, a, b) == i) {
                    uint32_t idx = universal_hashing(hashcodes[k], size, aa, bb);
                    if (init_elements[idx] != NULL) {
                        free(init_elements);
                        if (retries == MAX_RETRIES - 1) {
                            for (uint32_t p = 0; p < i; p++) {
                                free_sec_htbl((void *) fst_slots[p].elements, fst_slots[p].size,
                                        free_func);
                            }
                            free(fst_slots);
                            *rc = sec_uvhash_collided;
                            return NULL;
                        } else {
                            break;
                        }
                    } else {
                        init_elements[idx] = malloc(sizeof(kv_set[k].vlen));
                        if (init_elements == NULL)
                            abort();

                        memcpy(init_elements[idx], kv_set[k].value, kv_set[k].vlen);
                    }
                }
            }

            if (k == len) {
                sec_htbl_t init_slot = { .size = size, .a = aa, .b = bb, .elements =
                        (void * const *) init_elements };
                memcpy(&(fst_slots[i]), &init_slot, sizeof(sec_htbl_t));
                break;
            }
        }
    }

    perf_htbl_t init_perf_htbl = { .size = len, .a = a, .b = b, .fst_slots = fst_slots,
            .keyhash_func = keyhash_func, .keycmp_func = keycmp_func, .free_func = free_func };
    perf_htbl_t *perf_htbl = malloc(sizeof(perf_htbl_t));
    if (perf_htbl == NULL)
        abort();

    memcpy(perf_htbl, &init_perf_htbl, sizeof(perf_htbl_t));

    return perf_htbl;
}

void perf_htbl_destroy(perf_htbl_t *tbl) {
    for (uint32_t i = 0; i < tbl->size; i++) {
        free_sec_htbl((void **) tbl->fst_slots[i].elements, tbl->fst_slots[i].size, tbl->free_func);
    }

    free(tbl);
    tbl = NULL;
}

void *perf_htbl_get(perf_htbl_t *tbl, void *key) {
    uint32_t hashcode = tbl->keyhash_func(key);
    uint32_t slot = universal_hashing(hashcode, tbl->size, tbl->a, tbl->b);
    if (tbl->fst_slots[slot].size == 0)
        return NULL;
    uint32_t idx = universal_hashing(hashcode, tbl->fst_slots[slot].size, tbl->fst_slots[slot].a,
            tbl->fst_slots[slot].b);
    return tbl->fst_slots[slot].elements[idx];
}
