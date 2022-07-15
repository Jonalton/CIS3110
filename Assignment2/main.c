#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedule.h"

int checkDetailed(int argc,char* argv[]);
int checkVerbose(int argc,char* argv[]);
int checkRR(int argc,char* argv[]);
int getTimeQuantum(int argc,char* argv[]);

int main(int argc,char *argv[]){
    int detailed = checkDetailed(argc,argv);
    int verbose = checkVerbose(argc,argv);
    int robin = checkRR(argc,argv);
    int timeQuantum = getTimeQuantum(argc,argv);


    int numProcess,threadSwitch,processSwitch;
    struct process *p = readFile(&numProcess,&threadSwitch,&processSwitch);
    heap *h = initializeHeap(p,numProcess);  
    //printHeap(h);

    if (robin == 0){
        if (detailed == 0 && verbose == 0){
            simpleFCFS(h,numProcess,threadSwitch,processSwitch);
        }
        else if (detailed == 1 && verbose == 0){
            detailedFCFS(h,numProcess,threadSwitch,processSwitch);
        }
        else if (detailed == 0 && verbose == 1){
            verboseFCFS(h,numProcess,threadSwitch,processSwitch);
        }
    }

    else if (robin == 1){
        if (detailed == 0 && verbose == 0){
            simpleRR(h,numProcess,threadSwitch,processSwitch,timeQuantum);
        }
        else if (detailed == 1 && verbose == 0){
            //detailedRR(h,numProcess,threadSwitch,processSwitch);
        }
        else if (detailed == 0 && verbose == 1){
            //verboseRR(h,numProcess,threadSwitch,processSwitch);
        }
    }
    
    return -1;
}

int checkDetailed(int argc,char* argv[]){
    int detailed = 0;
    for (int i = 0;i < argc; i++){

        if (strcmp("-d",argv[i]) == 0){
            detailed = 1;
        }
    }
    return detailed;
}

int checkVerbose(int argc,char* argv[]){
    int verbose = 0;
    for (int i = 0;i < argc; i++){
        if (strcmp("-v",argv[i]) == 0){
            verbose = 1;
        }
    }
    return verbose;
}

int checkRR(int argc,char* argv[]){
    int robin = 0;
    for (int i = 0;i < argc; i++){
        if (strcmp("-r",argv[i]) == 0){
            robin = 1;
        }
    }
    return robin;
}

int getTimeQuantum(int argc,char* argv[]){
    int timeQuantum = 0;
    for (int i = 0;i < argc; i++){
        if (strcmp("-r",argv[i]) == 0){
            timeQuantum = atoi(argv[i+1]);
        }
    }
    return timeQuantum;
}