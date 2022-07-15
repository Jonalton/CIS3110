#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*struct process{
    int totalThreads;
    int processNum;
    int *threadNum;
    int *executionBursts;
    int *arrivalTime;
    struct thread **threads; 
};

struct thread{
    int processNumber;
    int threadNum;
    int CPU_time;
    int IO_time;
    int state[5];
};*/

struct process{
    int totalThreads;
    int processNum;
    struct thread *threads; 
};

struct thread{
    int processNum;
    int threadNum;
    int arrivalTime;
    int executionBurst;
    int finishTime;
    int startTime;
    int clock;
    int *exec;
    int *CPU_time;
    int *IO_time;
    int state;
    
};

typedef struct process process;
typedef struct thread thread;

struct process *readFile(int *numProcess,int *threadSwitch,int *processSwitch);
void printData(struct process *p,int numProcess,int threadSwitch,int processSwitch);