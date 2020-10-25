/*--------------------------------------------------------------------*/
/* functions.c                                                        */
/* Author: Manos Lourakis                                             */
/*--------------------------------------------------------------------*/

#include "functions.h"

/* Prints shell prompt */
void printPrompt(){
    char* name;
    char cwd[PATH_MAX];

    struct passwd *pass = getpwuid(getuid());
    name = pass->pw_name;
    strcat(name, "@cs345sh");

    if(getcwd(cwd, sizeof(cwd)) != NULL)
        strcat(name, cwd);
    else
        perror("getcwd() error");

    strcat(name, "/$ ");
    printf("%s", name);
}

/* Reads line from stdin */
char* readLine(){
    int buffer_size = RL_BUFSIZE;
    int pos = 0;
    char* buffer = malloc(buffer_size * sizeof(char));
    int c;

    if(!buffer){
        perror("Allocation error\n");
        exit(1);
    }

    while(1){
        c = getchar();
        if(c == EOF || c == '\n'){
            buffer[pos] = '\0';
            return buffer;
        }
        else{
            buffer[pos] = c;
        }
        pos++;

        if(pos > buffer_size){
            buffer_size += RL_BUFSIZE;
            buffer = realloc(buffer, buffer_size);
            if(!buffer){
                perror("Allocation error\n");
                exit(1);
            }
        }
    }
}

/* Split input into tokens */
char** tokenizeLine(char* input, char* c){
    char* token;
    char** command;
    char* separator = c;
    int index = 0;
    int buffer_size = COMMAND_BUFF_SIZE;

    command = malloc(buffer_size * sizeof(char*));
    if(command == NULL){
        perror("malloc failed");
        exit(1);
    }
    
    token = strtok(input, separator);
    while(token != NULL){
        command[index] = token;
        index++;

        /* Reallocate the arguments list if necessary */
        if(index > buffer_size){
            buffer_size += COMMAND_BUFF_SIZE;
            command = realloc(command, buffer_size * sizeof(char*));
        }

        token = strtok(NULL, separator);
    }

    /* NULL termination of the arguments list */
    command[index] = NULL;
    return command;
}

/* Terminates shell */
int exitShell(){
    return 0;
}

/* Shell built-in cd command */
int cd(char *path){
    return chdir(path);
}

/* Loads and executes a shell command that needs redirection */
void execRedirection(char** buffer, int mode){
    int fd;
    FILE* fp;
    char** command;
    pid_t child_pid;
    removeSpaces(buffer[1]);

    command = tokenizeLine(buffer[0], " ");
    child_pid = fork();

    if(child_pid == 0){

        switch (mode)
        {
        case INPUT:
            fp = fopen(buffer[1], "r");
            break;
        case OUTPUT:
            fp = fopen(buffer[1], "w+");
            break;
        case APPEND:
            fp = fopen(buffer[1], "a");
            break;
        }

        if(!fp){
            perror("Cannot open file\n");
            return;   
        }

        fd = fileno(fp);

        switch (mode)
        {
        case INPUT:
            dup2(fd, 0);
            break;
        case OUTPUT:
            dup2(fd, 1);
            break;
        case APPEND:
            dup2(fd, 1);
            break;
        }

        execvp(command[0], command);
        printf("This won't be printed if execvp is successful\n");
    }
    else if(child_pid < 0){
        perror("Fork failed\n");
        exit(1);
    }
    else{
        wait(NULL);
    }
}

/* Loads and ececutes pipeline commands */
void execPipedCommand(char* buffer){
    int procsNum = 0;
    char** commands;
    char** proc_command;
    commands = tokenizeLine(buffer, ">");

    while(commands[procsNum] != NULL){
        printf("Commands[%d] = %s\n", procsNum, commands[procsNum]);
        procsNum++;
    }
    int i;
    pid_t pid;

    int fd[procsNum][2]; // 2 file descriptors for each process
    for(i = 0; i < procsNum; i++){
        proc_command = tokenizeLine(commands[i], " ");
        //Dont create pipe when in last process
        if(i != procsNum - 1){
            if(pipe(fd[i]) == -1){
                perror("Error in pipe creation\n");
                return;
            }
        }
        pid = fork();
        if(pid == 0){
            if(i != procsNum - 1){
                dup2(fd[i][1], 1);
                close(fd[i][0]);
                close(fd[i][1]);
            }
            if( i != 0){
                dup2(fd[i-1][0], 0);
                close(fd[i-1][0]);
                close(fd[i-1][1]);
            }
            execvp(proc_command[0], proc_command);
            perror("Invalid input");
            exit(1);
        }
        else{
            close(fd[i-1][0]);
            close(fd[i-1][1]);
        }
        wait(NULL);
    }
}

/* Loads and executes a simple shell command */
void execSimple(char** buffer){
    int stat_loc;
    pid_t child_pid;

    child_pid = fork();
    
    // The child process
    if(child_pid == 0){
        if(execvp(buffer[0], buffer) < 0){
            perror(buffer[0]);
            exit(1);
        }
        printf("This won't be printed if execvp is successful\n");
    }
    else if (child_pid < 0){
        perror("Fork failed\n");
        exit(1);
    }
    // Parent process
    else{
        waitpid(child_pid, &stat_loc, WUNTRACED);
    }
}

/* Function to remove all spaces from a given string */
void removeSpaces(char* str) {
    int i;
    int count = 0;
    for(i = 0; str[i]; i++){
        if (str[i] != ' '){
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}