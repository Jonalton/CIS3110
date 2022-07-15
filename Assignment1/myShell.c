#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <string.h>
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include "myShell.h"


int main (int argc, char *argv[]){
	char *buffer;
    char *filename;
    size_t length = 256;
    size_t read;
    int index = 0;
    pid_t child;
    int wstatus;
    int background = 0;
    int output = 0;
    int input = 0;
    int pipe = 0;

    buffer = malloc(sizeof(char)*length);
    filename = malloc(sizeof(char));

	printf("> ");
	read = getline(&buffer, &length, stdin);
    //PARSE INPUT
    char** parameters = parseInput(buffer,&index);

    while(strncmp(buffer,"exit",4) != 0 && strncmp(buffer,"quit",4) != 0){       
        background = 0;
        input = 0;
        output = 0;
        pipe = 0;

        for (int i=0;i<index;i++){

            if (parameters[i] != NULL){

                if (strcmp(parameters[i],">") == 0){
                    output = 1;
                    filename = realloc(filename,sizeof(char)*(strlen(parameters[i+1])+1));
                    strcpy(filename,parameters[i+1]);
                    parameters[i] = NULL;
                    parameters[i+1] = NULL;

                }

                else if (strcmp(parameters[i],"<") == 0){
                    input = 1;
                    filename = realloc(filename,sizeof(char)*(strlen(parameters[i+1])+1));
                    strcpy(filename,parameters[i+1]);
                    parameters[i] = NULL;
                    parameters[i+1] = NULL;
                }
                else if (strcmp(parameters[i],"&") == 0){
                    background = 1;
                    parameters[i] = NULL;
                }
                else if (strcmp(parameters[i],"|") == 0){
                    pipe = 1;
                    parsePipe(parameters,i,index);
                }
            }
            
        }
        if (pipe != 1){
            child = createChild(parameters[0],parameters,background,input,output,filename);
        }   
                    
        //SCAN NEXT LINE
        printf("> ");
        read = getline(&buffer, &length, stdin);
        index = 0; 
        parameters = parseInput(buffer, &index);


    }
    printf("myShell terminating...\n\n");
    free(buffer);
    for (int i = 0; i < index;i++){
        free(parameters[i]);
    }
    free(parameters);
    free(filename);
    printf("[Process Completed]\n");
    exit(0);

	return -1;
}


char** parseInput(char* buffer,int *index){
    //remove newline from input
    int length = strlen(buffer);
    buffer[length-1] = '\0';

    char** parameters;
    char* token;
    char space[2] = " ";
    parameters = malloc(sizeof(char*));
    
    token = strtok(buffer,space);

    while(token!= NULL){ 
        parameters = realloc(parameters, sizeof(char*)*(*index+1));
        parameters[*index] = malloc(sizeof(char)*(strlen(token)+1));
        strcpy(parameters[*index],token);
        token = strtok(NULL,space);
        ++*index;
    }
    parameters = realloc(parameters, sizeof(char*)*(*index+1));
    parameters[*index] = NULL;

    return parameters;
}


//function parses commands for a pipe
void parsePipe(char** parameters,int i,int index){
    char **first;
    char **second;
    int z = 0;
    int j;
    first = malloc(sizeof(char*));
    second = malloc(sizeof(char*));

    for (j=0;j<i;j++){
        first = realloc(first,sizeof(char*)*(j+1));
        first[j] = malloc(sizeof(char)*strlen(parameters[j])+1);
        strcpy(first[j],parameters[j]);
    }
    for (int k=(i+1);k<index;k++){
        second = realloc(second,sizeof(char*)*(k+1));
        second[z] = malloc(sizeof(char)*strlen(parameters[k])+1);
        strcpy(second[z],parameters[k]);
        z++;
    }
    createPipe(first,second,j,z);
}

//creates pipe call and double forks
void createPipe(char** first, char** second,int j,int z){
    int status;
    int fd[2];
    pid_t childpid;

    int READ_END = 0;
    int WRITE_END = 1;


    if (pipe(fd) == -1){
        fprintf(stderr,"Pipe failed\n");
        exit(-1);
    }

    childpid = fork();

    if (childpid >= 0){
        //child
        if (childpid == 0){
            //writes output from first command
            dup2(fd[WRITE_END],STDOUT_FILENO);
            close(fd[READ_END]);
            close(fd[WRITE_END]);
            status = execvp(first[0],first);

            if(status == -1){
                if (errno == EACCES){
                    printf("Permission denied\n");
                }
                else{
                    perror("execvp");
                }
            }
            exit(status);
        }
        else{
            //grandchild
            childpid = fork();
            if (childpid >= 0){
                if (childpid == 0){
                    //reads output from first child
                    dup2(fd[READ_END],STDIN_FILENO);
                    close(fd[WRITE_END]);
                    close(fd[READ_END]);
                    //executes second command
                    status = execvp(second[0],second);

                    if(status == -1){
                        if (errno == EACCES){
                            printf("Permission denied\n");
                        }
                        else{
                            perror("execvp");
                        }
                    }
                    exit(status);
                }

                //parent
                else{
                    close(fd[READ_END]);
                    close(fd[WRITE_END]);
                    waitpid(childpid,&status,0);
                }
            }
            else{
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }
    }
    else{
        perror("fork");
        exit(EXIT_FAILURE);
    }
    

    //free strings
    for (int k=0;k<j;k++){
        free(first[k]);
    }
    free(first);
    for(int k=0;k<z;k++){
        free(second[k]);
    }
    free(second);
}

//creates child process
pid_t createChild(char* command, char**parameters,int background,int input,int output, char *filename){

    int status;
    char pathbuf[1000];
    FILE* fp;
    //printf("%s\n",command);
    pid_t childpid = fork();
    if (childpid >= 0){
        if (childpid == 0){
            //printf("Parent process %d child process %d\n",getppid(),getpid());
            //confstr(_CS_PATH, pathbuf, 1000);
	        //printf ( "PATH = %s\n", pathbuf );

            if (output == 1){
                fp = freopen(filename,"w+",stdout);
                status = execvp(command,parameters);
                fclose(fp);
            }
            else if (input == 1){
                fp = freopen(filename,"r+",stdin);
                status = execvp(command,parameters);
                fclose(fp);
            }

            else{
                status = execvp(command,parameters);
            }

            if(status == -1){
                if (errno == EACCES){
                    printf("Permission denied\n");
                }
                else{
                    perror("execvp");
                }
            }
            exit(status);
        }
        else{
            if (background == 0){
                //printf("Parent %d waiting for %d\n",getpid(),childpid);
                waitpid(childpid,&status,0);
            }
            else{
                //printf("Parent %d running with child %d\n",getpid(),childpid);
            }
            

        }
        
    }
    else{
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

