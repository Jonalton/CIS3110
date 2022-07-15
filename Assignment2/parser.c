
#include "parser.h"

 struct process *readFile(int *numProcess,int *threadSwitch,int *processSwitch){
    char file[1000];
    int c = fgetc(stdin);
    int i = 0;

    while (!feof(stdin)){
        //printf("%c\n",(char)c);
        file[i] = (char)c;
        c = fgetc(stdin);   
        i++;

    }
    file[i] = '\0';
    //printf("file\n%s\n",file);

    int pos = 0;
    int newpos = 0;
    struct process *p = malloc(sizeof(struct process));


    sscanf(file,"%d %d %d%n\n",&(*numProcess),&(*threadSwitch),&(*processSwitch),&pos);
    //printf("first: %d %d %d\n",(*numProcess),(*threadSwitch),(*processSwitch));
    p = realloc(p,sizeof(struct process)*(*numProcess));

    for (int i = 0;i<(*numProcess);i++){
        sscanf(file + pos,"%d %d%n",&p[i].processNum,&p[i].totalThreads,&newpos);
        //printf("%d %d\n",p[i].processNum,p[i].totalThreads);
        p[i].threads = malloc(sizeof(struct thread)*p[i].totalThreads);
        pos += newpos;

        for(int j=0;j<p[i].totalThreads;j++){

            sscanf(file + pos,"%d %d %d%n",&p[i].threads[j].threadNum,&p[i].threads[j].arrivalTime,&p[i].threads[j].executionBurst,&newpos);
            p[i].threads[j].exec = malloc(sizeof(struct thread)*p[i].threads[j].executionBurst);
            p[i].threads[j].CPU_time = malloc(sizeof(struct thread)*p[i].threads[j].executionBurst);
            p[i].threads[j].IO_time = malloc(sizeof(struct thread)*(p[i].threads[j].executionBurst-1));

            
            p[i].threads[j].processNum = p[i].processNum;
            p[i].threads[j].state = 0;
        
            pos += newpos;
    
            //printf("Thread: %d %d %d\n",p[i].threads[j].threadNum,p[i].threads[j].arrivalTime,p[i].threads[j].executionBurst);


            for (int k=0;k<p[i].threads[j].executionBurst;k++){
                if (k == p[i].threads[j].executionBurst - 1){
                    sscanf(file+pos,"%d %d%n",&p[i].threads[j].exec[k],&p[i].threads[j].CPU_time[k],&newpos);
                    p[i].threads[j].IO_time[k] = 0;
                    pos += newpos;
                    //printf("Exec: %d %d\n",p[i].threads[j].exec[k],p[i].threads[j].CPU_time[k]);
                }
                else{
                    sscanf(file+pos,"%d %d %d%n",&p[i].threads[j].exec[k],&p[i].threads[j].CPU_time[k],&p[i].threads[j].IO_time[k],&newpos);
                    pos += newpos;
                    //printf("Exec: %d %d %d\n",p[i].threads[j].exec[k],p[i].threads[j].CPU_time[k],p[i].threads[j].IO_time[k]);
                }

            }
        }
    }
    return p;
}

void printData(struct process *p,int numProcess,int threadSwitch,int processSwitch){
    printf("%d %d %d\n",numProcess,threadSwitch,processSwitch);

    for (int i = 0;i<numProcess;i++){
        printf("%d %d\n",p[i].processNum,p[i].totalThreads);
        for (int j=0;j<p[i].totalThreads;j++){
            printf("%d %d %d\n",p[i].threads[j].threadNum,p[i].threads[j].arrivalTime,p[i].threads[j].executionBurst);
            for (int k=0;k<p[i].threads[j].executionBurst;k++){
                if (k == p[i].threads[j].executionBurst-1){
                    printf("%d %d\n",p[i].threads[j].exec[k],p[i].threads[j].CPU_time[k]);
                }
                else{
                    printf("%d %d %d\n",p[i].threads[j].exec[k],p[i].threads[j].CPU_time[k],p[i].threads[j].IO_time[k]);
                }
            }
        }
    }
}
