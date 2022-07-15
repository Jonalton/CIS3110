#include "parser.h"

typedef struct node{
    int start;
    process p;
    struct node *next;
}node_t;



void print_list(node_t * head);
void push(node_t * head, process p,int start); 
process pop(node_t ** head);
int getLength(node_t *head);




node_t *createProcessQueue(data d);
node_t *createMemoryQueue(process p);