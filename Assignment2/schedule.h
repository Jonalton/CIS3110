#include "heap.h"


void simpleFCFS(heap *h,int numProcess,int threadSwitch,int processSwitch);
thread *FCFSthreads(heap *h,int numProcess,int threadSwitch,int processSwitch);
void detailedFCFS(heap *h,int numProcess,int threadSwitch,int processSwitch);
void verboseFCFS(heap *h,int numProcess,int threadSwitch,int processSwitch);
void verboseStatementFCFS(int old,int newS, int size, thread t);
int isRunning(int size, thread *t);
int isReady(int size, thread *t);
int isTerminated(int size, thread t);
int isBlocked(int size, thread t);



void simpleRR(heap *h,int numProcess,int threadSwitch,int processSwitch,int timeQuantum);


