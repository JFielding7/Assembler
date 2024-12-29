#include "hashtable.h"

#include <stdlib.h>

#include "linked_list.h"

hash_table *hash_table_new(size_t element_size, size_t (*hash_func)(void *)) {
    hash_table *ht = malloc(sizeof(ht));
    ht->len = 0;
    ht->ht = vec_zero(INITIAL_CAPACITY, element_size);
    ht->hash_func = hash_func;
    return ht;
}

void hash_table_double_capacity(hash_table *ht) {
    vec *old_ht = ht->ht;
    size_t capacity = old_ht->capacity;
    vec *new_ht = vec_fill_zero(linked_list*, capacity << 1);
    ht->ht = new_ht;

    for (size_t i = 0; i < capacity; i++) {
        linked_list *bucket = vec_get(old_ht, linked_list*, i);

    }

    vec_free(ht->ht);
    ht->ht = new_ht;
}

linked_list *get_bucket(hash_table *ht, void *value, size_t *bucket_index) {
    *bucket_index = ht->hash_func(value) & ht->ht->capacity - 1;
    return vec_get(ht->ht, linked_list*, *bucket_index);
}

void hash_table_insert(hash_table *ht, void *value) {
    if (ht->len << 1 > ht->ht->capacity) {
        hash_table_double_capacity(ht);
    }

    size_t bucket_index;
    linked_list *bucket = get_bucket(ht, value, &bucket_index);

    if (bucket == NULL) {
        bucket = linked_list_new();
        vec_set(ht->ht, linked_list*, bucket_index, bucket);
    }

    linked_list_push_back(bucket, value);
    ht->len++;
}

bool hash_table_contains(hash_table *ht, void *value, int (*cmp)(void*, void*)) {
    size_t bucket_index;
    return linked_list_contains(get_bucket(ht, value, &bucket_index), value, cmp);
}
