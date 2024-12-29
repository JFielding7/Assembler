#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

linked_list *linked_list_new() {
    linked_list *list = malloc(sizeof(linked_list));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void linked_list_node_push_back(linked_list *list, node *n) {
    n->next = NULL;

    if (list->head == NULL) {
        list->head = n;
        list->tail = n;
    } else {
        list->tail->next = n;
        list->tail = n;
    }
}

void linked_list_push_back(linked_list *list, void *value) {
    node *n = malloc(sizeof(node));
    n->value = value;
    linked_list_node_push_back(list, n);
}

bool linked_list_contains(linked_list *list, void *value, int (*cmp)(void*, void*)) {
    if (list == NULL) {
        return false;
    }

    node *curr = list->head;
    while (curr != NULL) {
        if (cmp(value, curr->value) == 0) {
            return true;
        }

        curr = curr->next;
    }

    return false;
}

void linked_list_print(linked_list *list, char *format) {
    node *curr = list->head;
    while (curr != NULL) {
        printf(format, curr->value);
        printf("\n");
        curr = curr->next;
    }
}

void linked_list_free(linked_list *list) {
    if (list == NULL) {
        return;
    }

    node *curr = list->head;
    while (curr != NULL ) {
        node *next = curr->next;
        free(curr->value);
        free(curr);
        curr = next;
    }

    free(list);
}
