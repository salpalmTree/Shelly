#ifndef _SHELLY_FUNC_H
#define _SHELLY_FUNC_H
#define SHELLY_GL_BUFSIZE 64
#define MAX_FILE_AMOUNT 10

bool running = false;  
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

dirNode *head = NULL; 
// read input as "create data.txt"
// read input
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
            theInputString = realloc(theInputString, buffsize); 
            if(!theInputString) return NULL; 
        }
    }
    theInputString[position] = '\0'; 
    return theInputString; 
}
// string comparison function
bool equalStrings(char *stringOne, char *stringTwo)
{
    bool areEqual; 
    (strcmp(stringOne, stringTwo) != 0 ? (areEqual = false) : (areEqual = true)); 
    return areEqual; 
}
// sets command type
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
// file creation
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
        free(fileName); 
        char *userIn = getInput(); 
        fputs(userIn, newFile); 
        fclose(newFile);
        free(userIn); 
        return newFile; 
    }
}
// file deletion
void deleteFile(char *fileName)
{
    if(remove(fileName))
    {
        printf("Could not delete that file.\n"); 
    }
    free(fileName); 
}
// read file
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
// edit file (append)
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
// display choices
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
        for(int i = 0; i < MAX_FILE_AMOUNT; i++)
        {
            dirToAdd->files[i].fileName = "xemptyx"; 
        }
    }
    else
    {
        dirToAdd->dirName = dirName; 
        dirToAdd->next = *head; 
        dirToAdd->prev = (*head)->prev; 
         
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
    printf("Error finding that Direcotry."); 
    return NULL; 
}

void add_File_To_Dir(dirNode *head, char *fileName)
{
    while(head != NULL)
    {
        for(int i = 0; i < MAX_FILE_AMOUNT; i++)
        {
            if((strcmp(head->files[i].fileName, "xemptyx")) == 0)
            {
                FILE *newFile = createFile(fileName);
                if(!newFile)
                {
                    printf("ERR: File could not be created"); 
                }
                else
                {
                    printf("File created in %s directory as %s \n", head->dirName, head->files[i].fileName); 
                }
            }
        }
    }
}

void ls_Dir(dirNode *head)
{
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
        for(int i = 0; i < MAX_FILE_AMOUNT; i++)
        {
            if((strcmp(head->files[i].fileName, "xemptyx")) == 0)
            {
                break; 
            }
            else
            {
                printf("%s\n", head->files[i].fileName); 
                fileCount++; 
            }
        }
        if(fileCount == 0)
        {
            printf("No files in Directory.\n"); 
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
#endif