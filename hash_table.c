#include <stdio.h>


#include "hashtable.h"

#include <stdlib.h>

#include "linked_list.h"

hash_table *hash_table_alloc(size_t element_size, size_t (*hash_func)(void *), int (*cmp)(void*, void*)) {
    hash_table *ht = malloc(sizeof(ht));
    ht->len = 0;
    ht->ht = vec_zero(INITIAL_CAPACITY, element_size);
    ht->hash_func = hash_func;
    ht->cmp = cmp;
    return ht;
}

linked_list *get_bucket(hash_table *ht, void *value, size_t *bucket_index) {
    *bucket_index = ht->hash_func(value) & ht->ht->capacity - 1;
    return vec_get(ht->ht, linked_list*, *bucket_index);
}

void insert(hash_table *ht, void *value) {
    size_t bucket_index;
    linked_list *bucket = get_bucket(ht, value, &bucket_index);

    printf("Bucket %lu\n", bucket_index);

    if (bucket == NULL) {
        bucket = linked_list_new();
        vec_set(ht->ht, linked_list*, bucket_index, bucket);
    }

    linked_list_push_back(bucket, value);
    ht->len++;
}

void hash_table_double_capacity(hash_table *ht) {
    vec *old_ht = ht->ht;
    size_t capacity = old_ht->capacity;
    vec *new_ht = vec_fill_zero(linked_list*, capacity << 1);
    ht->ht = new_ht;

    for (size_t i = 0; i < capacity; i++) {
        linked_list *bucket = vec_get(old_ht, linked_list*, i);
        if (bucket != NULL) {
            node *curr = bucket->head;
            while (curr != NULL) {
                insert(ht, curr->value);
                curr = curr->next;
            }
        }
    }

    vec_free(ht->ht);
    ht->ht = new_ht;
}

void hash_table_insert(hash_table *ht, void *value) {
    if (ht->len << 1 > ht->ht->capacity) {
        hash_table_double_capacity(ht);
    }

    insert(ht, value);
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
