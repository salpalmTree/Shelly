#include "../inc/shelly_func.h"
char ** parse_command(char * line)
{
    int buffsize = SHELLY_GL_BUFSIZE, position = 0;
    char **tokens = (char **)malloc(sizeof(char *) * buffsize);
    char *token; 
    if(!tokens)
    {
        printf("Error allocating\n"); 
        exit(EXIT_FAILURE); 
    }
    token = strtok(line, SHELLY_DELI); 
    while(token != NULL)
    {
        if(position > 2)
        {
            printf("Not a valid command combo.\n");
            return NULL;
        }
        else
        { 
            tokens[position] = token; 
            position++;
            token = strtok(NULL, SHELLY_DELI); 
        }
    }
    tokens[position] = NULL;
    return tokens; 
}
char * getInput(void)
{
    int buffsize = SHELLY_GL_BUFSIZE; 
    int position = 0; 
    int c; 
    char *theInputString = (char *)malloc(sizeof(char) * buffsize);  
    if(!theInputString)
    {
        printf("Error allocating.\n"); 
        return NULL; 
    }
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
    (strcmp(stringOne, stringTwo) == 0 ? (areEqual = true) : (areEqual = false)); 
    return areEqual; 
}
aCommand commandType(char *userInputString)
{
    if(equalStrings(userInputString, "delete"))
    {
        return DELETE; 
    }
    else if(equalStrings(userInputString, "create"))
    {
        return CREATE;
    }
    else if(equalStrings(userInputString, "set"))
    {
        return SET; 
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
    else if(equalStrings(userInputString, "clear"))
    {
        return CLEAR; 
    }
    else if(equalStrings(userInputString, "help"))
    {
        return HELP;
    }
    else if(equalStrings(userInputString, "ls"))
    {
        return LS;
    }
    return CONT; 
}
FILE * createFile(char *fileName)
{
    FILE *newFile; 
    if((newFile = fopen(fileName, "w")) == NULL) {
        printf("File could not be created.\n");
        return NULL;
    }
    else
    {
        printf("A file has been created %s\n", fileName); 
        printf("To file ->"); 
        char *userIn = getInput(); 
        fputs(userIn, newFile); 
        fclose(newFile);
        return newFile; 
    }
}
void deleteFile(char *fileName)
{
    if(remove(fileName))
    {
        printf("Could not delete that file.\n"); 
    }
}
void readFile(char *fileName)
{
    FILE *fileToRead; 
    int c;
    if((fileToRead = fopen(fileName, "r")) == NULL) 
    {
        printf("File doesn't exit.\n"); 
    }
    else
    { 
        while((c = getc(fileToRead)) != EOF)
        {
            putchar(c); 
        }
        printf("\n"); 
        fclose(fileToRead); 
    }
}
void editFile(char *fileName)
{
    FILE *fileToAppend; 
    if((fileToAppend = fopen(fileName, "a")) == NULL)
    {
        printf("That file could not be edited.\n");
    }
    else
    { 
        printf("File being appended %s\n", fileName); 
        printf("To File -> "); 
        char *userIn = getInput(); 
        fputs(userIn, fileToAppend); 
        fclose(fileToAppend); 
    }
}
void command_options(void)
{
    printf("Enter a [command] [obj. Type] [obj. Name]\n");
    printf("Commands can include:\n");
    printf("\tcreate, delete, edit, set, ls, clear, and exit\n"); 
    printf("Obj. Types are:\n"); 
    printf("\t'file', and 'dir'\n");
    printf("Some tips:"); 
    printf("\n\t'set ..' goes up one directory."); 
    printf("\n\t'clear dir [directory_name]' removes all files in [directory_name].");
    printf("\n\t'ls' lists files and directories in the set directory.\n\n"); 
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
        *head = dirToAdd;
    }
    else
    {
        dirNode *temp = *head; 
        dirToAdd->dirName = dirName;
        while(temp->next != NULL)
        {
            temp = temp->next; 
        }
        temp->next = dirToAdd;
        dirToAdd->prev = temp;
        dirToAdd->next = NULL; 
    }
    for(int i = 0; i < MAX_FILE_AMOUNT; i++)
    {
        dirToAdd->files[i].fileName = EMPTY_FILE; 
    }
    
    printf("Directory %s created\n", dirName); 
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
    int8_t i; 
    if(head == NULL)
    {
        printf("No valid directory found.\n");
    }
    else
    {
        for(i = 0; i < MAX_FILE_AMOUNT; i++)
        {
            if(equalStrings(head->files[i].fileName, EMPTY_FILE))
            {
                if((head->files[i].file = createFile(fileName)))
                {
                    head->files[i].fileName = fileName; 
                    printf("File '%s' created in '%s' directory.\n", head->files[i].fileName, head->dirName); 
                    createdFile = true; 
                    break; 
                }
                else
                {
                    printf("File could not be created.\n"); 
                    break; 
                }
            }
        }
        if(i == (MAX_FILE_AMOUNT) && createdFile == false) // for loop still increases i to 10 
        {
            printf("Directory is full.\n");
            ls_Dir(head); 
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
            printf(">%s\n", head->next->dirName); 
        }
        while(i < MAX_FILE_AMOUNT)
        {
            if((strcmp(head->files[i].fileName, EMPTY_FILE)) != 0)
            {
                printf("~ %s\n", head->files[i].fileName);
                fileCount++; 
            }
            i++; 
        }
    }
}
int find_file(dirNode *head, char *fileName)
{
    for(int i = 0; i < MAX_FILE_AMOUNT; i++)
    {
        if(equalStrings(fileName, head->files[i].fileName))
        {
            return i; 
        }
    }
    return INT8_MIN;
}
dirNode * back_one_directory(dirNode *head) 
{
    if(head->prev == NULL)
    {
        printf("Can't go up a directory.\n"); 
        return head; 
    }
    return head->prev; 
}
void rm_directory(dirNode *head, char *dir_name)
{
    if(head->next == NULL)
    {
        printf("Cannot delete '%s' directory, but can delete all files.\n", head->dirName); 
        rm_all_files(head); 
    }
    else if(head->next != NULL && head->next->next != NULL && equalStrings(head->next->dirName, dir_name))
    {
        printf("Directory '%s' has been deleted.\n", head->next->dirName); 
        rm_all_files(head->next); 
        dirNode *temp = head->next; 
        head->next = temp->next; 
        temp->next->prev = head;  
        free(temp); 
    }
    else if(head->next->prev != NULL && head->next->next == NULL && equalStrings(head->next->dirName, dir_name))
    {
        printf("Directory '%s' has been deleted.\n", head->next->dirName); 
        rm_all_files(head->next); 
        dirNode *temp = head->next; 
        head->next = NULL; 
        free(temp); 
    }
    else
    {
        printf("Can't delete directory.\n"); 
    }
}
void rm_all_files(dirNode *head)
{
    for(int i = 0; i < MAX_FILE_AMOUNT; i++)
    {
        if(!(equalStrings(head->files[i].fileName, EMPTY_FILE)))
        {
            deleteFile(head->files[i].fileName);
            head->files[i].fileName = EMPTY_FILE; 
        }
    }
    printf("Files removed from '%s'.\n", head->dirName); 
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
            if(head->next == NULL)
            {
                printf("\n"); 
                while (head != NULL)
                {
                    printf("%s <-", head->dirName); 
                    head = head->prev;
                }
            }
        }
        if(head == NULL)
        {
            printf("NULL"); 
        }
    }
}



