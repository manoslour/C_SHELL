/*--------------------------------------------------------------------*/
/* shell.c                                                            */
/* Author: Manos Lourakis                                             */
/*--------------------------------------------------------------------*/

#include "functions.h"

int main(int argc, char *argv[]){

    char* input;
    char** command;

    while(1){

        printPrompt();
        input = readLine();
        if(strstr(input, ">")){
            execPipedCommand(input);
        }
        else if(strstr(input, "|||")){
            command = tokenizeLine(input, "|||");
            execRedirection(command, APPEND);
        }
        else if(strstr(input, "||")){
            command = tokenizeLine(input, "||");
            execRedirection(command, OUTPUT);
        }
        else if(strstr(input, "|")){
            command = tokenizeLine(input, "|");
            execRedirection(command, INPUT);
        }
        else
        {
            command = tokenizeLine(input, " ");

            // Handle empty commands
            if(!command[0]){
                free(input);
                free(command);
                continue;
            }
            else if(strcmp(command[0], "cd") == 0){
                if(cd(command[1]) < 0){
                    perror(command[1]);
                }
            }
            else if(strcmp(command[0], "exit") == 0){
                return exitShell();
            }
            else{
                execSimple(command);
            }
        }
        
        //free(input);
        //free(command);
    }
    return 0;
}