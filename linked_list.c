#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

linked_list *linked_list_new() {
    linked_list *list = malloc(sizeof(linked_list));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void linked_list_push_back(linked_list *list, void *value) {
    node *n = malloc(sizeof(node));
    n->value = value;
    n->next = NULL;

    if (list->head == NULL) {
        list->head = n;
        list->tail = n;
    }

    list->tail->next = n;
    list->tail = n;
}

bool linked_list_contains(linked_list *list, void *value, int (*cmp)(void*, void*)) {
    node *curr = list->head;
    while (curr != NULL) {
        if (cmp(value, curr->value) == 0) {
            return true;
        }

        curr = curr->next;
    }

    return false;
}


