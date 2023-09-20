#ifndef _SHELLY_FUNC_H
#define _SHELLY_FUNC_H
#define SHELLY_GL_BUFSIZE 64
#define MAX_FILE_AMOUNT 10
#define SHELLY_DELI " \n\t\r\a"
#define EMPTY_FILE "xemptyx"
#define File "file"
#define Dir "dir"

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum command {
    EXT = 0,
    DELETE, // delete file or dir 
    CREATE, // create file or dir 
    READ, // read file 
    EDIT, // edit/append file edit dir name?
    SET, // set a dir 
    CONT, // continue execution
    HELP, // help
    CLEAR, // clear all files from a directory
    LS, // list directory
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
char ** parse_command(char * line);
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
void editFile(char *fileName);
// display choices
void command_options(void);
// add a directory
void add_Dir(dirNode **head, char *dirName);
// look up a directory 
dirNode * lookup_Dir(dirNode *head, char *dirName);
// add file to directory
void add_File_To_Dir(dirNode *head, char *fileName);
// list a directory
void ls_Dir(dirNode *head);
// is a file in the directory
int find_file(dirNode *head, char *fileName); 
// equivalent to 'cd ..' goes back one directory
dirNode * back_one_directory(dirNode *head);
// remove a directory
void rm_directory(dirNode *head, char *dir_name); 
// new command processing function
void process_command(char **userCommandIn); 
// print a Directory for debugging
void printDir(dirNode *head);
// remove all files from a directory
void rm_all_files(dirNode *head); 
#endif
