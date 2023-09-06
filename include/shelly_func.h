#ifndef _SHELLY_FUNC_H
#define _SHELLY_FUNC_H
#define SHELLY_GL_BUFSIZE 64
#define MAX_FILE_AMOUNT 10
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum command {
    EXT = 0,
    DELETE,
    CREATE, 
    READ,
    EDIT,
    CONT,
}aCommand; 

typedef struct fileNode {
    char *fileName; 
    FILE *file; 
}fileNode; 

typedef struct dirNode {
    char *dirName;
    struct dirNode *next;
    struct dirNode *prev;
    struct fileNode files[MAX_FILE_AMOUNT]; 
}dirNode; 

// read input as "create data.txt"
// read input
char * getInput(void);
// string comparison function
bool equalStrings(char *stringOne, char *stringTwo); 
// sets command type
aCommand commandType(char *userInputstring);
// file creation
FILE * createFile(char *fileName);
// file deletion
void deleteFile(char *fileName);
// read file
void readFile(char *fileName);
// edit file (append)
FILE * editFile(char *fileName);
// display choices
void displayChoices(void);
// add a directory
void add_Dir(dirNode **head, char *dirName);
// look up a directory 
dirNode * lookup_Dir(dirNode *head, char *dirName);
// add file to directory
void add_File_To_Dir(dirNode *head, char *fileName);
// list a directory
void ls_Dir(dirNode *head);
// print a Directory for debugging
void printDir(dirNode *head);
#endif
