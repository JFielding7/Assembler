#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdbool.h>

typedef struct linked_list_s {
    struct node_s *head;
    struct node_s *tail;
} linked_list;

typedef struct node_s {
    void *value;
    struct node_s *next;
} node;

linked_list *linked_list_new();

void linked_list_push_back(linked_list *list, void *value);

bool linked_list_contains(linked_list *list, void *value, int (*cmp)(void*, void*));

#endif //LINKED_LIST_H
