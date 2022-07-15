#include "schedule.h"
#include <math.h>


void simpleFCFS(heap *h,int numProcess,int threadSwitch,int processSwitch){
    int CPU_time = 0;
    int CPU_util = 0;
    printf("FCFS Scheduling\n");
    heap *temp = h;
    int totalTime = 0;
    int size = heapSize(h);
    thread *t = malloc(sizeof(thread)*size);
    int i = 0;
    while(!isEmpty(temp)){
        t[i] = removeMin(temp);
        //printf("arrivalTime: %d\n",t[i].arrivalTime);
        if (t[i].processNum != t[i-1].processNum && i != 0){
            totalTime += processSwitch;
        }
        else if (t[i].threadNum != t[i-1].threadNum && i != 0){
            totalTime += threadSwitch;
        }
        for (int j=0;j<t[i].executionBurst;j++){
            totalTime += t[i].CPU_time[j] + t[i].IO_time[j];
            CPU_time += t[i].CPU_time[j];
        }
        i++;
    }
    printf("Total time required is %d time units\n",totalTime);
    printf("Average Turnaround Time is %d time units\n",totalTime/numProcess);
    CPU_util = (int)((float)CPU_time/(float)totalTime*100);
    printf("CPU Utilization is %d%%\n",CPU_util);
}

void detailedFCFS(heap *h,int numProcess,int threadSwitch,int processSwitch){
    int size = heapSize(h);
    thread *t = FCFSthreads(h,numProcess,threadSwitch,processSwitch);

    int totalTime = 0;
    int turnaround = 0;
    int service = 0;
    int io = 0;
    for (int i=0;i<size;i++){
        turnaround = 0;
        service = 0;
        io = 0;
        for (int j=0;j<t[i].executionBurst;j++){
            turnaround += t[i].CPU_time[j] + t[i].IO_time[j];
            totalTime += t[i].CPU_time[j] + t[i].IO_time[j];
            service += t[i].CPU_time[j];
            io += t[i].IO_time[j];
        }
        if (t[i].processNum != t[i-1].processNum && i != 0){
            totalTime += processSwitch;
        }
        else if (t[i].threadNum != t[i-1].threadNum && i != 0){
            totalTime += threadSwitch;
        }    
        printf("Thread %d of Process %d:\n",t[i].threadNum,t[i].processNum);
        printf("    arrival time: %d\n",t[i].arrivalTime);
        printf("    service time: %d, I/O time: %d, turnaround time: %d, finish time:%d\n",service,io,turnaround,totalTime);
 
    }
}

void verboseFCFS(heap *h,int numProcess,int threadSwitch,int processSwitch){
    int size = heapSize(h);
    int prevTime = 0;
    int newTime = prevTime;
    thread *t = FCFSthreads(h,numProcess,threadSwitch,processSwitch);

    //printf("size: %d\n",size);
    for (int i =0; i<size;i++){
        //printf("start Time %d arrivalTime %d\n",t[i].startTime,t[i].arrivalTime);
        //printf("prevTime %d\n",prevTime);
        if (prevTime == 0){
            t[i].state = 1;
            verboseStatementFCFS(0,t[i].state,size,t[i]);
        }

        if (isRunning(size,t)){
            t[i].state = 3;
            verboseStatementFCFS(0,t[i].state,size,t[i]);
        }
        
        for (int j=0; j<t[i].executionBurst;j++){
            newTime += t[i].CPU_time[j] + t[i].IO_time[j];
            //verboseStatementFCFS(prevTime,newTime,size,t);

        }
        //newTime += prevTime;
        if (t[i].processNum != t[i-1].processNum && i != 0){
            newTime += processSwitch;

        }
        else if (t[i].threadNum != t[i-1].threadNum && i != 0){
            newTime += threadSwitch;

        }
        t[i].state = 4;
        verboseStatementFCFS(1,t[i].state,size,t[i]);
        prevTime = newTime;

    }

}

void verboseStatementFCFS(int old, int newS, int size, thread t){
    char states[6][10] = {
        "new",
        "ready",
        "running",
        "blocked",
        "terminated"
    };
    printf("At time %d: Thread %d of Process %d moves from %s to %s\n",t.finishTime,t.threadNum,t.processNum,states[old],states[newS]);
    


}

int isRunning(int size, thread *t){
    int running = 0;
    for (int i = 0; i < size; i++){
        if (t[i].state == 2){
            running = 1;
        }
    }
    return running;
}

int isReady(int size, thread *t){
    int ready = 0;
    for (int i = 0; i < size; i++){
        if (t[i].state == 1){
            ready = 1;
        }
    }
    return ready;
}

int isTerminated(int size, thread t){
    int term = 0;
    if (t.state == 4){
            term = 1;
    }
    return term;
}

int isBlocked(int size, thread t){
    int block = 0;
    if (t.state == 4){
            block = 1;
    }
    return block;
}


thread *FCFSthreads(heap *h,int numProcess,int threadSwitch,int processSwitch){
    int CPU_time = 0;
    int CPU_util = 0;
    printf("FCFS Scheduling\n");
    heap *temp = h;
    int totalTime = 0;
    int size = heapSize(h);
    thread *t = malloc(sizeof(thread)*size);
    int i = 0;
    while(!isEmpty(temp)){
        t[i] = removeMin(temp);

        t[i].startTime = totalTime;

        //printf("arrivalTime: %d\n",t[i].arrivalTime);
        if (t[i].processNum != t[i-1].processNum && i != 0){
            totalTime += processSwitch;
        }
        else if (t[i].threadNum != t[i-1].threadNum && i != 0){
            totalTime += threadSwitch;
        }
        for (int j=0;j<t[i].executionBurst;j++){
            totalTime += t[i].CPU_time[j] + t[i].IO_time[j];
            CPU_time += t[i].CPU_time[j];    
        }
        t[i].finishTime = totalTime;

        i++;
    }
    printf("Total time required is %d time units\n",totalTime);
    printf("Average Turnaround Time is %d time units\n",totalTime/numProcess);
    CPU_util = (int)((float)CPU_time/(float)totalTime*100);
    printf("CPU Utilization is %d%%\n",CPU_util);
    return t;
}






//****************ROUND ROBIN****************/

void simpleRR(heap *h,int numProcess,int threadSwitch,int processSwitch, int timeQuantum){
    int CPU_time = 0;
    int CPU_util = 0;
    printf("RR Scheduling (Time Quantum = %d time units\n",timeQuantum);
    heap *temp = h;
    int totalTime = 0;
    int size = heapSize(h);
    int quantumCount = 0;
    int burstTime = 0;

    thread *t = malloc(sizeof(thread)*size);
    int i = 0;
    int newArrival = 0;
    while(!isEmpty(temp)){
        t[i] = removeMin(temp);
        burstTime = 0;
        quantumCount = 0;

        while (quantumCount < 10){

            quantumCount++;
        }
        //printf("arrivalTime: %d\n",t[i].arrivalTime);
        if (t[i].processNum != t[i-1].processNum && i != 0){
            totalTime += processSwitch;
        }
        else if (t[i].threadNum != t[i-1].threadNum && i != 0){
            totalTime += threadSwitch;
        }
        for (int j=0;j<t[i].executionBurst;j++){
            
            totalTime += t[i].CPU_time[j] + t[i].IO_time[j];
            burstTime = t[i].CPU_time[j] + t[i].IO_time[j];
            CPU_time += t[i].CPU_time[j];
            quantumCount++;
        }

        i++;
    }
    printf("Total time required is %d time units\n",totalTime);
    printf("Average Turnaround Time is %d time units\n",totalTime/numProcess);
    CPU_util = (int)((float)CPU_time/(float)totalTime*100);
    printf("CPU Utilization is %d%%\n",CPU_util);
}    

