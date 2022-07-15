#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%d\n", current->p.size);
        current = current->next;
    }
}

void push(node_t * head, process p,int start){
    node_t * current = head;

    while (current->next != NULL) {
        current = current->next;
    }
    /* now we can add a new variable */
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->start = start;
    current->next->p = p;
    current->next->next = NULL;
}

process pop(node_t ** head) {
    process retp;
    node_t * next_node = NULL;

    if (*head == NULL) {
        return retp;
    }

    next_node = (*head)->next;
    retp = (*head)->p;
    free(*head);
    *head = next_node;

    return retp;
}

int getLength(node_t *head){
    node_t * current = head;
    int size = 0;

    while (current != NULL) {
        current = current->next;
        size++;
    }
    return size;
}

node_t *createProcessQueue(data d){
    node_t *head = malloc(sizeof(node_t));
    head->p = d.p[0];
    head->next = NULL;
    for (int i = 1; i<d.count; i++){
        push(head,d.p[i],0);
    }
    return head;
}
