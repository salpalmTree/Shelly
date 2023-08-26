#ifndef _SHELLY_FUNC_H
#define _SHELLY_FUNC_H

enum command {
    EXT = 0,
    DELETE,
    CREATE, 
    READ,
    EDIT,
    CONT,
}commands; 


// read input
char * getInput()
{
    char *theInputString = (char *)malloc(sizeof(char) * 81);  
    if(theInputString == NULL) return NULL; 
    char *sIn = theInputString;
    while((*theInputString++ = getchar()) != '\n');
    *theInputString = '\0'; 
    return sIn; 
}
// string comparison function
bool equalStrings(char *stringOne, char *stringTwo)
{
    bool areEqual;
    while(*stringOne == *stringTwo && *stringOne != '\0' && *stringTwo != '\0')
    { 
        stringOne++; 
        stringTwo++; 
    }
    if(*stringOne == '\0' && *stringTwo == '\0')
    {
        areEqual = true; 
    }
    else
    {
        areEqual = false; 
    }
    return areEqual; 
}
// sets command type
enum command commandType(char *userInputString)
{
    if(equalStrings(userInputString, "delete\n"))
    {
        return DELETE; 
    }
    else if(equalStrings(userInputString, "create\n"))
    {
        return CREATE;
    }
    else if(equalStrings(userInputString, "read\n"))
    {
        return READ; 
    }
    else if(equalStrings(userInputString, "edit\n"))
    {
        return EDIT;
    }
    else if(equalStrings(userInputString, "exit\n"))
    {
        return EXT; 
    }
    else
    {
        return CONT; 
    }
}
// file creation
FILE * createFile(char *fileName)
{
    FILE *newFile, *copyNewFile; 
    if((newFile = fopen(fileName, "w")) == NULL){printf("File could not be created.\n");return NULL;}
    else
    {
        copyNewFile = newFile; 
        printf("A file has been created %s", fileName); 
        printf("To file ->"); 
        char *userIn = getInput(); 
        fputs(userIn, newFile); 
        fclose(newFile);
        return copyNewFile; 
    }
}
// file deletion
void deleteFile(char *fileName)
{
    if(remove(fileName))
    {
        printf("Could not delete that file.\n"); 
    }
}
// read file
void readFile(char *fileName, FILE * file)
{
    if((fopen(fileName, "r")) == NULL) printf("File doesn't exit.\n"); 
    else
    {
        int c; 
        while((c = getc(file)) != EOF)
        {
            putchar(c); 
        }
        fclose(file); 
    }
}
// edit file (append)
FILE * editFile(char *fileName)
{
    FILE *fileToAppend, *copyFileToAppend; 
    if((fileToAppend = fopen(fileName, "a")) == NULL){printf("That file could not be edited.\n");return NULL;}
    else
    {
        copyFileToAppend = fileToAppend; 
        printf("File being appended %s", fileName); 
        printf("To File ->"); 
        char *userIn = getInput(); 
        fputs(userIn, copyFileToAppend); 
        fclose(copyFileToAppend); 
        return copyFileToAppend; 
    }
}
// display choices
void displayChoices()
{
    printf("- \tcreate.\n"); 
    printf("- \tread.\n"); 
    printf("- \tedit.\n"); 
    printf("- \tdelete.\n"); 
    printf("= \texit.\n"); 
}
#endif