
typedef struct process{
    int pid;
    int size;
    int load;
}process;

typedef struct data{
    process *p;
    int count;
}data;

data readFile(char* filename);
