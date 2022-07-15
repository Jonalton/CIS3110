#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

//main functions
void firstFit(int *memory,node_t *pQueue);
void bestFit(int *memory,node_t *pQueue);
void worstFit(int *memory,node_t *pQueue);
void nextFit(int *memory,node_t *pQueue);

//helper functions

int compareHoles(int *holes,int total,int size,int *memory,int *index);
int compareBestHoles(int *holes,int total,int size,int *memory,int *index);
int compareWorstHoles(int *holes,int total,int size,int *memory,int *index);
int *initializeMemory();
int *findHoles(int *memory, int *totalHoles);
int *findNextHoles(int *memory,int *totalHoles, int index);
int holeSize(int *memory, int index);
void fillMemory(int* memory,int start,int size);
void deleteMemory(int* memory,int start, int size);
int getMemUsage(int *memory);


int main(int argc, char*argv[]){
    data d = readFile(argv[1]);
    node_t *pQueue = createProcessQueue(d);
    int *memory = initializeMemory();


    if (strcmp(argv[2],"First") == 0){
        firstFit(memory,pQueue);
    }
    else if (strcmp(argv[2],"Best") == 0){
        bestFit(memory,pQueue);
    }
    else if (strcmp(argv[2],"Worst") == 0){
        worstFit(memory,pQueue);
    }
    else if (strcmp(argv[2],"Next") == 0){
        nextFit(memory,pQueue);
    }
    return -1;
}


//FIRST FIT
void firstFit(int *memory,node_t *pQueue){
    node_t *memQueue = malloc(sizeof(node_t));
    int index = 0;
    int init = 0;
    int totalHoles = 0;
    int totalMemory = 0;
    int totalProcesses = 0;
    int load = 0;
    int cumulativeMemory = 0;
    int cumHoles = 0;
    float avgProcesses = 0.0;
    float avgHoles = 0.0;



    while (pQueue != NULL){
        int *holes = findHoles(memory,&totalHoles);


        if (compareHoles(holes,totalHoles,pQueue->p.size,memory,&index)){
            cumHoles += totalHoles;
            process p = pop(&pQueue);
            p.load++;
            if (init == 0){
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
                init = 1;
            }
            else if (getLength(memQueue) == 0){
                memQueue = realloc(memQueue,sizeof(node_t));
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
            }
            else{
                push(memQueue,p,index);
            }
            fillMemory(memory,index,p.size);
            load++;
            totalMemory += getMemUsage(memory);
            cumulativeMemory = (int)((float)totalMemory/(float)load);
            totalProcesses += getLength(memQueue);
            printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",getLength(memQueue),totalHoles,getMemUsage(memory),cumulativeMemory);
        }


        //swap out oldest process
        else{
            int start = memQueue->start;
            process p = pop(&memQueue);
            if (p.load == 3){
                deleteMemory(memory,start,p.size);
            }
            else{
                deleteMemory(memory,start,p.size);
                push(pQueue,p,0);
            }
            index = start;
        }
    }



    avgProcesses = (float)totalProcesses/(float)load;
    avgHoles = (float)cumHoles/(float)load;
    printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%memory = %d\n",load,avgProcesses,avgHoles,cumulativeMemory);  
}

//BEST FIT
void bestFit(int *memory,node_t *pQueue){
    node_t *memQueue = malloc(sizeof(node_t));
    int index = 0;
    int init = 0;
    int totalHoles = 0;
    int totalMemory = 0;
    int totalProcesses = 0;
    int load = 0;
    int cumulativeMemory = 0;
    int cumHoles = 0;
    float avgProcesses = 0.0;
    float avgHoles = 0.0;


    while (pQueue != NULL){
        int *holes = findHoles(memory,&totalHoles);

        //FIND BEST FITTING PROCESS FOR EACH HOLE
        if (compareBestHoles(holes,totalHoles,pQueue->p.size,memory,&index)){
            cumHoles += totalHoles;
            process p = pop(&pQueue);
            p.load++;
            if (init == 0){
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
                init = 1;
            }
            else if (getLength(memQueue) == 0){
                memQueue = realloc(memQueue,sizeof(node_t));
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
            }
            else{
                push(memQueue,p,index);
            }
            fillMemory(memory,index,p.size);
            load++;
            totalMemory += getMemUsage(memory);
            cumulativeMemory = (int)((float)totalMemory/(float)load);
            totalProcesses += getLength(memQueue);
            printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",getLength(memQueue),totalHoles,getMemUsage(memory),cumulativeMemory);
        }


        //swap out oldest process
        else{
            int start = memQueue->start;
            process p = pop(&memQueue);
            if (p.load == 3){
                deleteMemory(memory,start,p.size);
            }
            else{
                deleteMemory(memory,start,p.size);
                push(pQueue,p,0);
            }
            index = start;
        }
    }


    avgProcesses = (float)totalProcesses/(float)load;
    avgHoles = (float)cumHoles/(float)load;

    printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%memory = %d\n",load,avgProcesses,avgHoles,cumulativeMemory);  
}

//WORST FIT
void worstFit(int *memory,node_t *pQueue){
    node_t *memQueue = malloc(sizeof(node_t));
    int index = 0;
    int init = 0;
    int totalHoles = 0;
    int totalMemory = 0;
    int totalProcesses = 0;
    int load = 0;
    int cumulativeMemory = 0;
    int cumHoles = 0;
    float avgProcesses = 0.0;
    float avgHoles = 0.0;

    
    while (pQueue != NULL){
        int *holes = findHoles(memory,&totalHoles);

        //FIND WORST FITTING PROCESS FOR EACH HOLE
        if (compareWorstHoles(holes,totalHoles,pQueue->p.size,memory,&index)){
            cumHoles += totalHoles;
            process p = pop(&pQueue);
            p.load++;
            if (init == 0){
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
                init = 1;
            }
            else if (getLength(memQueue) == 0){
                memQueue = realloc(memQueue,sizeof(node_t));
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
            }
            else{
                push(memQueue,p,index);
            }
            fillMemory(memory,index,p.size);
            load++;
            totalMemory += getMemUsage(memory);
            cumulativeMemory = (int)((float)totalMemory/(float)load);
            totalProcesses += getLength(memQueue);
            printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",getLength(memQueue),totalHoles,getMemUsage(memory),cumulativeMemory);
        }


        //swap out oldest process
        else{
            int start = memQueue->start;
            process p = pop(&memQueue);
            if (p.load == 3){
                deleteMemory(memory,start,p.size);
            }
            else{
                deleteMemory(memory,start,p.size);
                push(pQueue,p,0);
            }
            index = start;
        }
    }



    avgProcesses = (float)totalProcesses/(float)load;
    avgHoles = (float)cumHoles/(float)load;

    printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%memory = %d\n",load,avgProcesses,avgHoles,cumulativeMemory);  
}

//NEXT FIT
void nextFit(int *memory,node_t *pQueue){
    node_t *memQueue = malloc(sizeof(node_t));
    int index = 0;
    int init = 0;
    int totalHoles = 0;
    int totalMemory = 0;
    int totalProcesses = 0;
    int load = 0;
    int cumulativeMemory = 0;
    int cumHoles = 0;
    float avgProcesses = 0.0;
    float avgHoles = 0.0;

    
    while (pQueue != NULL){
        int *holes = findNextHoles(memory,&totalHoles,index);

        //FIND BEST FITTING PROCESS FOR EACH HOLE
        if (compareHoles(holes,totalHoles,pQueue->p.size,memory,&index)){
            cumHoles += totalHoles;
            process p = pop(&pQueue);
            p.load++;
            if (init == 0){
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
                init = 1;
            }
            else if (getLength(memQueue) == 0){
                memQueue = realloc(memQueue,sizeof(node_t));
                memQueue->p = p;
                memQueue->next = NULL;
                memQueue->start = index;
            }
            else{
                push(memQueue,p,index);
            }
            fillMemory(memory,index,p.size);
            load++;
            totalMemory += getMemUsage(memory);
            cumulativeMemory = (int)((float)totalMemory/(float)load);
            totalProcesses += getLength(memQueue);
            printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",getLength(memQueue),totalHoles,getMemUsage(memory),cumulativeMemory);
        }


        //swap out oldest process
        else{
            int start = memQueue->start;
            process p = pop(&memQueue);
            if (p.load == 3){
                deleteMemory(memory,start,p.size);
            }
            else{
                deleteMemory(memory,start,p.size);
                push(pQueue,p,0);
            }
            index = 0;
        }
    }



    avgProcesses = (float)totalProcesses/(float)load;
    avgHoles = (float)cumHoles/(float)load;

    printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%memory = %d\n",load,avgProcesses,avgHoles,cumulativeMemory);  
}


//find first hole large enough to fit process 
int compareHoles(int *holes,int total,int size,int *memory,int *index){
    for (int i = 0; i<total; i++){
        if (size < holeSize(memory,holes[i])){
            *index = holes[i];
            //printf("hole size %d size %d\n",holeSize(memory,holes[i],index),size);
            return 1;
        }
    }
    return 0;
}

//order holes from smallest to largest, compare each process size and choose the tightest fit
int compareBestHoles(int *holes,int total,int size,int *memory,int *index){
    //find smallest hole size and compare with size of process
    int tempSize = 0;
    //sort
    for (int i = 0; i<total-1; i++){
        if(holeSize(memory,holes[i]) > holeSize(memory,holes[i+1])){
            tempSize = holes[i];
            holes[i] = holes[i+1];
            holes[i+1] = tempSize;
        }
    }
    for (int i = 0; i<total; i++){
        if (size < holeSize(memory,holes[i])){
            *index = holes[i];
            //printf("hole size %d size %d\n",holeSize(memory,holes[i],index),size);
            return 1;
        }
    }
    return 0;
}


//order holes from largest to smallest, compare each process size and choose the tightest fit
int compareWorstHoles(int *holes,int total,int size,int *memory,int *index){
    //find largest hole size and compare with size of process
    int tempSize = 0;
    //sort
    for (int i = 0; i<total-1; i++){
        if(holeSize(memory,holes[i]) < holeSize(memory,holes[i+1])){
            tempSize = holes[i];
            holes[i] = holes[i+1];
            holes[i+1] = tempSize;
        }
    }


    for (int i = 0; i<total; i++){
        if (size < holeSize(memory,holes[i])){
            *index = holes[i];
            //printf("hole size %d size %d\n",holeSize(memory,holes[i],index),size);
            return 1;
        }
    }
    return 0;
}

int getMemUsage(int *memory){
    int storage = 0;
    for (int i = 0; i<1024;i++){
        if (memory[i] == 1){
            storage++;
        }
    }
    int usage = (int)((float)storage/(float)1024*100);
    return usage;
}

int *initializeMemory(){
    int *memory = malloc(sizeof(int)*1024);
    for(int i=0;i<1024;i++){
        memory[i] = 0;
    }
    return memory;
}

int *findHoles(int *memory,int *totalHoles){
    int *holes = malloc(sizeof(int));
    int size = 0;
    int empty = 0;
    for (int i = 0; i < 1024; i++){
        if (memory[i] == 0 && empty == 0){
            holes[size] = i;
            empty = 1;
            size++;
            holes = realloc(holes,sizeof(int)*(size+1));
        }
        if (memory[i] == 1 && empty == 1){
            empty = 0;
        }
    }
    *totalHoles = size;
    return holes;
}

int *findNextHoles(int *memory,int *totalHoles, int index){
    int *holes = malloc(sizeof(int));
    int size = 0;
    int empty = 0;
    for (int i = index; i < 1024; i++){
        if (memory[i] == 0 && empty == 0){
            holes[size] = i;
            empty = 1;
            size++;
            holes = realloc(holes,sizeof(int)*(size+1));
        }
        if (memory[i] == 1 && empty == 1){
            empty = 0;
        }
    }
    *totalHoles = size;
    return holes;
}

int holeSize(int *memory, int index){
    int count = 0;
    for (int i = index; i <1024; i++){
        if (memory[i] == 1){
            return count;
        }
        count++;
    }
    return count;
}

void fillMemory(int* memory,int start, int size){
    for (int i = start;i<(start+size);i++){
        memory[i] = 1;
    }
}

void deleteMemory(int* memory,int start, int size){
    for (int i = start;i<(start+size);i++){
        memory[i] = 0;
    }
}

