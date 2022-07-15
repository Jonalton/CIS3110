#include "parser.h"

struct heap{
    thread *t;
    int total;
    int count;
};
typedef struct heap heap;


heap *createHeap(int totalThreads);
void insertItem(thread t,heap *h);
thread removeMin(heap *heap);

int minKey(heap *h);
thread minElement(heap *h);
int heapSize(heap *h);
int isEmpty(heap *h);

void upheap(int index, heap *h);
void downheap(int index, heap *h);

void printHeap(heap *h);
heap* initializeHeap(process* p,int numProcess);
