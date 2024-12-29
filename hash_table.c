#include "hashtable.h"

#include <stdlib.h>

#include "linked_list.h"

hash_table *hash_table_new(size_t (*hash_func)(void *)) {
    hash_table *ht = malloc(sizeof(ht));
    ht->ht = vec_new(linked_list*);
    ht->hash_func = hash_func;
    return ht;
}

size_t get_bucket(hash_table *ht, void *value, linked_list **bucket) {
    size_t bucket_index = ht->hash_func(value) & ht->ht->capacity - 1;
    *bucket = vec_get(ht->ht, linked_list*, bucket_index);
    return bucket_index;
}

void hash_table_insert(hash_table *ht, void *value) {
    linked_list *bucket;
    size_t bucket_index = get_bucket(ht, value, &bucket);

    if (bucket == NULL) {
        bucket = linked_list_new();
        vec_set(ht->ht, linked_list*, bucket_index, bucket);
    }

    linked_list_push_back(bucket, value);
}

void hash_table_contains(hash_table *ht, void *value) {
    linked_list *bucket;
    size_t bucket_index = get_bucket(ht, value, &bucket);



    linked_list_push_back(bucket, value);
}
