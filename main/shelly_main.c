#include "../include/shelly_func.h"

bool running = false;  
dirNode *head = NULL; 
int main()
{
    add_Dir(&head, getInput());
    add_Dir(&head, getInput()); 
    add_Dir(&head, getInput()); 
    ls_Dir(lookup_Dir(head, getInput())); 
    //add_File_To_Dir(lookup_Dir(head, getInput()), getInput()); 
    //ls_Dir(head);

    printf("Welcome to Shelly... a super basic file shell thing.\n");   
    FILE *userFile; 
    char *userInputCommand; 
    char *userInput; 

    do
    {
        displayChoices();
        userInputCommand = getInput(); 
        switch (commandType(userInputCommand))
        {
        case CREATE:
            userInput = getInput(); 
            userFile = createFile(userInput); 
        break;
        case READ:
            userInput = getInput(); 
            readFile(userInput);
        break; 
        case EDIT:
            userInput = getInput(); 
            userFile = editFile(userInput);
        break; 
        case DELETE:
            userInput = getInput(); 
            deleteFile(userInput); 
        break; 
        case EXT:
            printf("Exiting...\n"); 
            running = false; 
        break;
        default:
            printf("Please choose a valid choice.\n"); 
        break;
        }
    }while (running == true);

    return 0; 
}

char * getInput(void)
{
    int buffsize = SHELLY_GL_BUFSIZE; 
    int position = 0; 
    int c; 
    char *theInputString = (char *)malloc(sizeof(char) * buffsize);  
    if(!theInputString) return NULL; 
    while((c = getchar()) != '\n')
    {
        theInputString[position] = c; 
        position++; 
        if(position >= buffsize)
        {
            buffsize += SHELLY_GL_BUFSIZE; 
            theInputString = (char*)realloc(theInputString, buffsize); 
            if(!theInputString) return NULL; 
        }
    }
    theInputString[position] = '\0'; 
    return theInputString; 
}

bool equalStrings(char *stringOne, char *stringTwo)
{
    bool areEqual; 
    (strcmp(stringOne, stringTwo) != 0 ? (areEqual = false) : (areEqual = true)); 
    return areEqual; 
}

aCommand commandType(char *userInputString)
{
    if(equalStrings(userInputString, "delete"))
    {
        free(userInputString);
        return DELETE; 
    }
    else if(equalStrings(userInputString, "create"))
    {
        return CREATE;
    }
    else if(equalStrings(userInputString, "read"))
    {
        return READ; 
    }
    else if(equalStrings(userInputString, "edit"))
    {
        return EDIT;
    }
    else if(equalStrings(userInputString, "exit"))
    {
        return EXT; 
    }
    else
    {
        return CONT; 
    }
}


FILE * createFile(char *fileName)
{
    FILE *newFile; 
    if((newFile = fopen(fileName, "w")) == NULL) {
        printf("File could not be created.\n");
        free(fileName); 
        return NULL;
    }
    else
    {
        printf("A file has been created %s\n", fileName); 
        printf("To file ->"); 
        //free(fileName); // Might free() too much
        char *userIn = getInput(); 
        fputs(userIn, newFile); 
        fclose(newFile);
        free(fileName); 
        free(userIn); 
        return newFile; 
    }
}

void deleteFile(char *fileName)
{
    if(remove(fileName))
    {
        printf("Could not delete that file.\n"); 
    }
    free(fileName); 
}

void readFile(char *fileName)
{
    FILE *fileToRead; 
    int c;
    if((fileToRead = fopen(fileName, "r")) == NULL) 
    {
        printf("File doesn't exit.\n"); 
        free(fileName); 
    }
    else
    { 
        while((c = getc(fileToRead)) != EOF)
        {
            putchar(c); 
        }
        free(fileName); 
        fclose(fileToRead); 
    }
}

FILE * editFile(char *fileName)
{
    FILE *fileToAppend; 
    if((fileToAppend = fopen(fileName, "a")) == NULL)
    {
        printf("That file could not be edited.\n");
        free(fileName); 
        return NULL;
    }
    else
    { 
        printf("File being appended %s\n", fileName); 
        printf("To File ->"); 
        free(fileName); 
        char *userIn = getInput(); 
        fputs(userIn, fileToAppend); 
        free(userIn); 
        fclose(fileToAppend); 
        return fileToAppend; 
    }
}

void displayChoices(void)
{
    printf("\n- \tcreate.\n"); 
    printf("- \tread.\n"); 
    printf("- \tedit.\n"); 
    printf("- \tdelete.\n"); 
    printf("= \texit.\n"); 
}

void add_Dir(dirNode **head, char *dirName)
{
    dirNode *dirToAdd = (dirNode*)malloc(sizeof(dirNode)); 
    if(!dirToAdd)
    {
        printf("Error creating Directory\n"); 
        free(dirToAdd); 
    }
    if(*head == NULL) // list is empty
    {
        dirToAdd->dirName = dirName; 
        dirToAdd->next = *head; 
        dirToAdd->prev = NULL;
    }
    else
    {
        dirToAdd->dirName = dirName; 
        dirToAdd->next = *head; 
        dirToAdd->prev = (*head)->prev; 
         
    }
    for(int i = 0; i < MAX_FILE_AMOUNT; i++)
    {
        dirToAdd->files[i].fileName = "xemptyx"; 
    }
    *head = dirToAdd;
}

dirNode * lookup_Dir(dirNode *head, char *dirName)
{
    while(head != NULL)
    {
        if(strcmp(head->dirName, dirName) == 0)
        {
            return head; 
        }
        head = head->next; 
    }
    printf("Error finding that Direcotry.\n"); 
    return NULL; 
}

void add_File_To_Dir(dirNode *head, char *fileName)
{
    bool createdFile = false; 
    if(head == NULL)
    {
        printf("Directory not found.\n");
    }
    else
    {
        while(!createdFile)
        {
            for(int i = 0; i < MAX_FILE_AMOUNT; i++)
            {
                if((strcmp(head->files[i].fileName, "xemptyx")) == 0)
                {
                    if(!(head->files[i].file = createFile(fileName)))
                    {
                        printf("File could not be created.\n"); 
                    }
                    else
                    {
                        head->files[i].fileName = fileName; 
                        printf("File created in %s directory as %s \n", head->dirName, head->files[i].fileName); 
                        createdFile = true; 
                        break; 
                    }
                }
            }
        }
    }
}

void ls_Dir(dirNode *head)
{
    int i = 0; 
    int fileCount = 0; 
    if(head == NULL)
    {
        printf("No Directory.\n"); 
    }
    else
    {
        printf("Name of Directory: %s\n", head->dirName);
        if(head->next != NULL)
        {
            printf("%s.dir\n", head->next->dirName); 
        }
        while(i < MAX_FILE_AMOUNT)
        {
            if((strcmp(head->files[i].fileName, "xemptyx")) != 0)
            {
                printf("%s\n", head->files[i].fileName);
                fileCount++; 
            }
            i++; 
        }
    }
}

void printDir(dirNode *head)
{
    if(head == NULL)
    {
        printf("The List is empty.\n"); 
    }
    else
    {
        while(head != NULL)
        {
            if(head->prev == NULL)
            {
                printf("NULL -> "); 
            }
            printf("%s -> ", head->dirName); 
            head = head->next; 
        }
        if(head == NULL)
        {
            printf("NULL"); 
        }
    }
}



