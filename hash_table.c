#include <stdio.h>


#include "hashtable.h"

#include <stdlib.h>

#include "linked_list.h"

hash_table *hash_table_alloc(size_t (*hash_func)(void *), int (*cmp)(void*, void*)) {
    hash_table *ht = malloc(sizeof(hash_table));
    ht->len = 0;
    ht->ht = vec_zero(INITIAL_CAPACITY, sizeof(linked_list*));
    ht->hash_func = hash_func;
    ht->cmp = cmp;
    return ht;
}

linked_list *get_bucket(hash_table *ht, void *value, size_t *bucket_index) {
    *bucket_index = ht->hash_func(value) & ht->ht->capacity - 1;
    return vec_get(ht->ht, linked_list*, *bucket_index);
}

void insert(hash_table *ht, node *n) {
    size_t bucket_index;
    linked_list *bucket = get_bucket(ht, n->value, &bucket_index);

    if (bucket == NULL) {
        bucket = linked_list_new();
        vec_set(ht->ht, linked_list*, bucket_index, bucket);
    }

    linked_list_node_push_back(bucket, n);
    ht->len++;
}

void rehash_elements(hash_table *ht, linked_list *bucket) {
    if (bucket == NULL) {
        return;
    }

    node *curr = bucket->head;
    while (curr != NULL) {
        insert(ht, curr);
        curr = curr->next;
    }

    free(bucket);
}

void hash_table_double_capacity(hash_table *ht) {
    vec *old_ht = ht->ht;
    size_t capacity = old_ht->capacity;
    ht->ht = vec_fill_zero(linked_list*, capacity << 1);

    for (size_t i = 0; i < capacity; i++) {
        rehash_elements(ht, vec_get(old_ht, linked_list*, i));
    }

    vec_free(old_ht);
}

void hash_table_insert(hash_table *ht, void *value) {
    if (ht->len << 1 > ht->ht->capacity) {
        hash_table_double_capacity(ht);
    }

    node *n = malloc(sizeof(node));
    n->value = value;
    insert(ht, n);
}

bool hash_table_contains(hash_table *ht, void *value) {
    size_t bucket_index;
    return linked_list_contains(get_bucket(ht, value, &bucket_index), value, ht->cmp);
}

void hash_table_print(hash_table *ht, char *format) {
    size_t capacity = ht->ht->capacity;
    for (size_t i = 0; i < capacity; i++) {
        linked_list *bucket = vec_get(ht->ht, linked_list*, i);
        if (bucket != NULL) {
            linked_list_print(bucket, format);
        }
    }
}

void hash_table_free(hash_table *ht) {
    for (size_t i = 0; i < ht->ht->capacity; i++) {
        linked_list_free(vec_get(ht->ht, linked_list*, i));
    }

    vec_free(ht->ht);
    free(ht);
}
