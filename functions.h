/*--------------------------------------------------------------------*/
/* functions.h                                                        */
/* Author: Manos Lourakis                                             */
/*--------------------------------------------------------------------*/

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>

#define RL_BUFSIZE 1024
#define COMMAND_BUFF_SIZE 64
#define INPUT 0
#define OUTPUT 1
#define APPEND 2

/* Prints shell prompt */
void printPrompt();

/* Reads line from stdin */
char* readLine();

/* Terminates shell */
int exitShell();

/* Split line into tokens */
char** tokenizeLine(char* input, char* c);

/* Shell built-in cd command */
int cd(char *path);

/* Loads and executes a simple shell command */
void execSimple(char** buffer);

/* Loads and executes a shell command that needs redirection */
void execRedirection(char** buffer, int mode);

/* Function to remove all spaces from a given string */
void removeSpaces(char *str);

#endif