char** parseInput(char* buffer,int *index);
pid_t createChild(char* command, char** parameters,int background,int input,int output, char *filename);
void parsePipe(char** parameters,int i,int index);
void createPipe(char** first, char** second,int j,int z);