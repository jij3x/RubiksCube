#include "comm_util.h"

dl_list_t *dl_list_create(size_t elem_size, free_fptr_t free_func) {
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

#define CRC_30_POLYNOMIAL 0x6030B9C7
#define CRC_30_SEED 0x3FFFFFFF

const uint32_t crc30_table[256] = { 0x00000000, 0x2030b9c7, 0x2051ca49, 0x0061738e, 0x20932d55,
        0x00a39492, 0x00c2e71c, 0x20f25edb, 0x2116e36d, 0x01265aaa, 0x01472924, 0x217790e3,
        0x0185ce38, 0x21b577ff, 0x21d40471, 0x01e4bdb6, 0x221d7f1d, 0x022dc6da, 0x024cb554,
        0x227c0c93, 0x028e5248, 0x22beeb8f, 0x22df9801, 0x02ef21c6, 0x030b9c70, 0x233b25b7,
        0x235a5639, 0x036aeffe, 0x2398b125, 0x03a808e2, 0x03c97b6c, 0x23f9c2ab, 0x240a47fd,
        0x043afe3a, 0x045b8db4, 0x246b3473, 0x04996aa8, 0x24a9d36f, 0x24c8a0e1, 0x04f81926,
        0x051ca490, 0x252c1d57, 0x254d6ed9, 0x057dd71e, 0x258f89c5, 0x05bf3002, 0x05de438c,
        0x25eefa4b, 0x061738e0, 0x26278127, 0x2646f2a9, 0x06764b6e, 0x268415b5, 0x06b4ac72,
        0x06d5dffc, 0x26e5663b, 0x2701db8d, 0x0731624a, 0x075011c4, 0x2760a803, 0x0792f6d8,
        0x27a24f1f, 0x27c33c91, 0x07f38556, 0x2824363d, 0x08148ffa, 0x0875fc74, 0x284545b3,
        0x08b71b68, 0x2887a2af, 0x28e6d121, 0x08d668e6, 0x0932d550, 0x29026c97, 0x29631f19,
        0x0953a6de, 0x29a1f805, 0x099141c2, 0x09f0324c, 0x29c08b8b, 0x0a394920, 0x2a09f0e7,
        0x2a688369, 0x0a583aae, 0x2aaa6475, 0x0a9addb2, 0x0afbae3c, 0x2acb17fb, 0x2b2faa4d,
        0x0b1f138a, 0x0b7e6004, 0x2b4ed9c3, 0x0bbc8718, 0x2b8c3edf, 0x2bed4d51, 0x0bddf496,
        0x0c2e71c0, 0x2c1ec807, 0x2c7fbb89, 0x0c4f024e, 0x2cbd5c95, 0x0c8de552, 0x0cec96dc,
        0x2cdc2f1b, 0x2d3892ad, 0x0d082b6a, 0x0d6958e4, 0x2d59e123, 0x0dabbff8, 0x2d9b063f,
        0x2dfa75b1, 0x0dcacc76, 0x2e330edd, 0x0e03b71a, 0x0e62c494, 0x2e527d53, 0x0ea02388,
        0x2e909a4f, 0x2ef1e9c1, 0x0ec15006, 0x0f25edb0, 0x2f155477, 0x2f7427f9, 0x0f449e3e,
        0x2fb6c0e5, 0x0f867922, 0x0fe70aac, 0x2fd7b36b, 0x3078d5bd, 0x10486c7a, 0x10291ff4,
        0x3019a633, 0x10ebf8e8, 0x30db412f, 0x30ba32a1, 0x108a8b66, 0x116e36d0, 0x315e8f17,
        0x313ffc99, 0x110f455e, 0x31fd1b85, 0x11cda242, 0x11acd1cc, 0x319c680b, 0x1265aaa0,
        0x32551367, 0x323460e9, 0x1204d92e, 0x32f687f5, 0x12c63e32, 0x12a74dbc, 0x3297f47b,
        0x337349cd, 0x1343f00a, 0x13228384, 0x33123a43, 0x13e06498, 0x33d0dd5f, 0x33b1aed1,
        0x13811716, 0x14729240, 0x34422b87, 0x34235809, 0x1413e1ce, 0x34e1bf15, 0x14d106d2,
        0x14b0755c, 0x3480cc9b, 0x3564712d, 0x1554c8ea, 0x1535bb64, 0x350502a3, 0x15f75c78,
        0x35c7e5bf, 0x35a69631, 0x15962ff6, 0x366fed5d, 0x165f549a, 0x163e2714, 0x360e9ed3,
        0x16fcc008, 0x36cc79cf, 0x36ad0a41, 0x169db386, 0x17790e30, 0x3749b7f7, 0x3728c479,
        0x17187dbe, 0x37ea2365, 0x17da9aa2, 0x17bbe92c, 0x378b50eb, 0x185ce380, 0x386c5a47,
        0x380d29c9, 0x183d900e, 0x38cfced5, 0x18ff7712, 0x189e049c, 0x38aebd5b, 0x394a00ed,
        0x197ab92a, 0x191bcaa4, 0x392b7363, 0x19d92db8, 0x39e9947f, 0x3988e7f1, 0x19b85e36,
        0x3a419c9d, 0x1a71255a, 0x1a1056d4, 0x3a20ef13, 0x1ad2b1c8, 0x3ae2080f, 0x3a837b81,
        0x1ab3c246, 0x1b577ff0, 0x3b67c637, 0x3b06b5b9, 0x1b360c7e, 0x3bc452a5, 0x1bf4eb62,
        0x1b9598ec, 0x3ba5212b, 0x3c56a47d, 0x1c661dba, 0x1c076e34, 0x3c37d7f3, 0x1cc58928,
        0x3cf530ef, 0x3c944361, 0x1ca4faa6, 0x1d404710, 0x3d70fed7, 0x3d118d59, 0x1d21349e,
        0x3dd36a45, 0x1de3d382, 0x1d82a00c, 0x3db219cb, 0x1e4bdb60, 0x3e7b62a7, 0x3e1a1129,
        0x1e2aa8ee, 0x3ed8f635, 0x1ee84ff2, 0x1e893c7c, 0x3eb985bb, 0x3f5d380d, 0x1f6d81ca,
        0x1f0cf244, 0x3f3c4b83, 0x1fce1558, 0x3ffeac9f, 0x3f9fdf11, 0x1faf66d6 };

uint32_t crc_30(uint32_t seed, uint8_t *buf_ptr, uint32_t len) {
    uint32_t data, crc_30;

    for (crc_30 = ~seed & 0x3FFFFFFF; len >= 8; len -= 8, buf_ptr++) {
        crc_30 = crc30_table[((crc_30 >> (30 - 8)) ^ *buf_ptr) & 0xff] ^ (crc_30 << 8);
    }

    if (len != 0) {
        data = ((uint32_t) (*buf_ptr)) << (30 - 8);
        while (len-- != 0) {
            if (((crc_30 ^ data) & (1L << 29)) != 0) {
                crc_30 <<= 1;
                crc_30 ^= CRC_30_POLYNOMIAL;
            } else {
                crc_30 <<= 1;
            }

            data <<= 1;
        }
    }

    crc_30 = ~crc_30;

    return (crc_30 & 0x3FFFFFFF);
}

const uint32_t PRIME_31B = ((uint64_t) 1 << 32) - 5;

uint32_t universal_hashing(uint32_t key, uint32_t slots, uint32_t a, uint32_t b) {
    return (((uint64_t) a * key + b) % PRIME_31B) % slots;
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

perf_htbl_t *perf_htbl_create(kv_pair_t kv_set[], size_t len, keyhash_fptr_t keyhash_func,
        keycmp_fptr_t keycmp_func, free_fptr_t free_func, int *rc) {
    assert(len > 0 && kv_set != NULL);

    srand(time(NULL));

    uint32_t hashcodes[len], ordered_hc[len];
    for (size_t i = 0; i < len; i++) {
        hashcodes[i] = keyhash_func(kv_set[i].key);
        ordered_hc[i] = hashcodes[i];
    }

    qsort(ordered_hc, len, sizeof(uint32_t), uint32_cmpfunc);
    for (size_t i = 1; i < len; i++) {
        if (ordered_hc[i - 1] == ordered_hc[i]) {
            uint32_t same = ordered_hc[i];
            size_t m, n;
            for (m = 0; m < len; m++) {
                if (hashcodes[m] == same)
                    break;
            }
            for (n = len - 1; n > 0; n--) {
                if (hashcodes[n] == same)
                    break;
            }

            if (keycmp_func(kv_set[m].key, kv_set[n].key) != 0)
                *rc = hash_key_collision;
            else
                *rc = duplicate_keys;

            return NULL;
        }
    }

    uint32_t a = (((uint64_t) rand() << 16 ^ rand()) | 1) % (PRIME_31B + 1);
    uint32_t b = ((uint64_t) rand() << 16 ^ rand()) % (PRIME_31B + 1);

    uint32_t count[len];
    for (size_t i = 0; i < len; i++) {
        count[i] = 0;
    }
    for (size_t i = 0; i < len; i++) {
        count[universal_hashing(hashcodes[i], len, a, b)]++;
    }

    sec_htbl_t *fst_slots = malloc(sizeof(sec_htbl_t) * len);

    for (size_t i = 0; i < len; i++) {
        sec_htbl_t init_sec_htbl = { .size = 0, .elements = NULL };
        memcpy(&(fst_slots[i]), &init_sec_htbl, sizeof(sec_htbl_t));

        if (count[i] == 0) {
            continue;
        }

        for (size_t retries = 0; retries < MAX_RETRIES; retries++) {
            uint32_t size = pow(count[i], 2);
            void **init_elements = malloc(sizeof(void *) * size);
            for (size_t j = 0; j < size; j++) {
                init_elements[j] = NULL;
            }

            uint32_t aa = (((uint64_t) rand() << 16 ^ rand()) | 1) % (PRIME_31B + 1);
            uint32_t bb = ((uint64_t) rand() << 16 ^ rand()) % (PRIME_31B + 1);

            size_t k = 0;
            for (; k < len; k++) {
                if (universal_hashing(hashcodes[k], len, a, b) == i) {
                    uint32_t idx = universal_hashing(hashcodes[k], size, aa, bb);
                    if (init_elements[idx] != NULL) {
                        free(init_elements);
                        if (retries == MAX_RETRIES - 1) {
                            for (size_t p = 0; p < i; p++) {
                                free_sec_htbl((void *) fst_slots[p].elements, fst_slots[p].size,
                                        free_func);
                            }
                            free(fst_slots);
                            *rc = secondary_uvhash_collision;
                            return NULL;
                        } else {
                            break;
                        }
                    } else {
                        init_elements[idx] = malloc(sizeof(kv_set[k].vlen));
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
