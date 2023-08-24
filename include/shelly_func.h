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
char * getInput(char *input)
{
    char *theInputString = (char *)malloc(sizeof(char) * 10);  
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
FILE * newFile(char *fileName, char *userInputString)
{
    if(commandType(userInputString) == CREATE)
    {
        return fopen(fileName, "w"); 
    }
}






#endif