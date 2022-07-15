#include "heap.h"
#include <math.h>

heap *createHeap(int totalThreads){
    heap *h = malloc(sizeof(heap));
    h->total = totalThreads;
    h->count = 0;
    h->t = malloc(sizeof(thread)*h->total);

    return h;
}



void insertItem(thread thread,heap *h){
    if (h->count < h->total){
        h->t[h->count] = thread;
        //re-order heap upheap
        upheap(h->count,h);
        h->count++;
    }
}


thread removeMin(heap *h){
    if (h->count == 0){
        printf("heap is empty\n");
        //return NULL;
    }
    thread t = h->t[0];
    h->t[0] = h->t[h->count-1];
    h->count--;
    downheap(0,h);
    //reorder read downheap
    return t;
}

int minKey(heap *h){
    return h->t[0].arrivalTime;
}

thread minElement(heap *h){
    return h->t[0];
}

int heapSize(heap *h){
    return h->total;
}

int isEmpty(heap *h){
    if (h->count == 0){
        return 1;
    }
    return 0;
}

void printHeap(heap *h){
    printf("**************print heap*****************\n");
    //int size = pow(2,index);
    //printf("test\n");
    for (int i=0;i<(h->total);i++){ 
        printf("%d --> ",h->t[i].arrivalTime);
    }
    printf("\n");

}

void upheap(int index,heap *h){
    thread temp;
    int parent = (index-1)/2;

    if(h->t[parent].arrivalTime > h->t[index].arrivalTime){
        temp = h->t[parent];
        h->t[parent] = h->t[index];
        h->t[index] = temp;
        upheap(parent,h);
    }
}

void downheap(int index, heap *h){
    int left = (index*2)+1;
    int right = (index*2)+2;
    int min;
    thread temp;

    // checks if left and right are a valid number between 0 and number of threads in heap
    if (left >= h->count || left < 0){
        left = -1;
    }
    if (right >= h->count || right < 0){
        right = -1;
    }

    //finds minimum arrival time
    if (left != -1 && h->t[left].arrivalTime < h->t[index].arrivalTime){
        min = left;
    }
    else{
        min = index;
    }
    if (right != -1 && h->t[right].arrivalTime < h->t[min].arrivalTime){
        min = right;
    }  

    //swap min with parent
    if (min != index){
        temp = h->t[min];
        h->t[min] = h->t[index];
        h->t[index] = temp;
        downheap(min,h);
    }

}

heap* initializeHeap(process *p,int numProcess){
    int totalThreads = 0;
    for (int i = 0;i<numProcess;i++){
        totalThreads = totalThreads + p[i].totalThreads;
    }
    heap *h = createHeap(totalThreads);
    //printf("total threads %d\n",h->total);
    for (int i = 0;i<numProcess;i++){
        for(int j=0;j<p[i].totalThreads;j++){
            insertItem(p[i].threads[j],h);
            //printf("i j %d %d\n",i,j);
        }
    }


    return h;
}