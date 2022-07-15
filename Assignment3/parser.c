#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


data readFile(char* filename){
    FILE *fp;
    process *p;

    int count = 0;
    p = malloc(sizeof(process)*count);
    fp = fopen(filename,"r");


    while (fscanf(fp,"%d %d",&p[count].pid,&p[count].size) == 2){
        //printf("%d %d\n",p[count].pid,p[count].size);
        p[count].load = 0;
        p = realloc(p,sizeof(process)*(count+2));
        count += 1;
    }
    data d;
    d.p = p;
    d.count = count;
    return d;
    
}

