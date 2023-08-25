#ifndef _SHELLY_FUNC_H
#define _SHELLY_FUNC_H

typedef enum command {
    ERR = 0,
    DELETE,
    CREATE, 
    OPEN,
    CLOSE,
    EDIT, 
    SHOW,
}command; 


// read input
char * getInput()
{
    char *theInputString = (char *)malloc(sizeof(char) * 50);  
    if(theInputString == NULL) return NULL; 
    char *sIn = theInputString;
    while((*sIn++ = getchar()) != '\n');
    *sIn++ = '\0'; 
    return theInputString; 
}
// string comparison function
bool equalStrings(char *stringOne, char *stringTwo)
{
    bool areEqual;
    while(*stringOne == *stringTwo && *stringOne != '\0' && *stringTwo != '\0')
    { 
        stringOne++; 
        stringTwo++; 
        if(*stringOne == '\0' && *stringTwo == '\0')
        {
            areEqual = true; 
        }
        else
        {
            areEqual = false; 
        }
    }
    return areEqual; 
}
// sets command type
command commandType(char *userInputString)
{
    if(equalStrings(userInputString, "delete"))
    {
        return DELETE; 
    }
    else if(equalStrings(userInputString, "create"))
    {
        return CREATE;
    }
    else if(equalStrings(userInputString, "open"))
    {
        return OPEN; 
    }
    else if(equalStrings(userInputString, "close"))
    {
        return CLOSE;
    }
    else if(equalStrings(userInputString, "edit"))
    {
        return EDIT;
    }
    else if(equalStrings(userInputString, "show"))
    {
        return SHOW; 
    }
    return ERR; 
}
// file creation
void createFile(char *fileName)
{
    FILE *newFile; 
    if((newFile = fopen(fileName, "w")) == NULL) printf("File could not be created.\n"); 
    else
    {
        printf("File [%s] has been created.\n", fileName); 
        printf("To file ->"); 
        char *userIn = getInput(); 
        fputs(userIn, newFile); 
        fclose(newFile);
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


#endif