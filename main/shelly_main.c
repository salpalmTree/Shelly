#include "../include/shelly_func.h"

bool running;  
dirNode *head = NULL;
char *userInput; 
char **parsed_command; 
int main(void)
{
    // NOTE: first init the shell, 
    //      then you start the loop shell
    //      lastly you clean up and free memory
    printf("Welcome to Shelly... a super basic file shell thing.\n");   
    FILE *userFile; 
    char *userInputCommand; 
    running = true;

    do {
        printf("  >"); 
        userInput = getInput(); 
        parsed_command = parse_command(userInput);
        process_command(parsed_command); 
    }while (running == true); 
    return 0; 
}

// Organizes a command in to be later processed
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

// Gets input from user, used for all input asks
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

// Check if two strings are equal
bool equalStrings(char *stringOne, char *stringTwo)
{
    bool areEqual; 
    (strcmp(stringOne, stringTwo) == 0 ? (areEqual = true) : (areEqual = false)); 
    return areEqual; 
}

// Choosing which command 
aCommand commandType(char *userInputString)
{
    if(equalStrings(userInputString, "delete"))
    {
        //free(userInputString);
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
    else if(equalStrings(userInputString, "help"))
    {
        return HELP;
    }
    return CONT; 
}

// Creates new file and expects contents to be put in
FILE * createFile(char *fileName)
{
    FILE *newFile; 
    if((newFile = fopen(fileName, "w")) == NULL) {
        printf("File could not be created.\n");
        //free(fileName); 
        return NULL;
    }
    else
    {
        printf("A file has been created %s\n", fileName); 
        printf("To file ->"); 
        char *userIn = getInput(); 
        fputs(userIn, newFile); 
        fclose(newFile);
        //free(fileName); 
        //free(userIn); 
        return newFile; 
    }
}

// Removes file
void deleteFile(char *fileName)
{
    if(remove(fileName))
    {
        printf("Could not delete that file.\n"); 
    }
    //free(fileName); 
}

// Opens file to be read and displays contents
void readFile(char *fileName)
{
    FILE *fileToRead; 
    int c;
    if((fileToRead = fopen(fileName, "r")) == NULL) 
    {
        printf("File doesn't exit.\n"); 
        //free(fileName); 
    }
    else
    { 
        while((c = getc(fileToRead)) != EOF)
        {
            putchar(c); 
        }
        //free(fileName); 
        fclose(fileToRead); 
    }
}

// Opens and appends given file
FILE * editFile(char *fileName)
{
    FILE *fileToAppend; 
    if((fileToAppend = fopen(fileName, "a")) == NULL)
    {
        printf("That file could not be edited.\n");
        //free(fileName); 
        return NULL;
    }
    else
    { 
        printf("File being appended %s\n", fileName); 
        printf("To File -> "); 
        //free(fileName); 
        char *userIn = getInput(); 
        fputs(userIn, fileToAppend); 
        //free(userIn); 
        fclose(fileToAppend); 
        return fileToAppend; 
    }
}

void command_options(void)
{
    printf("Enter a [command] [obj. Type] [obj. Name]\n");
    printf("Commands can include:\n");
    printf("\tcreate, delete, edit, set, and exit\n"); 
    printf("Obj. Types are:\n"); 
    printf("\t'file', and 'dir'\n");  
}

// Adds a new directory 
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
    printf("Directory %s created\n", dirName); 
}

// Looks up a specific directory and returns a pointer to it
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

// Creates file and add it to given directory
void add_File_To_Dir(dirNode *head, char *fileName)
{
    bool createdFile = false; 
    if(head == NULL)
    {
        printf("No valid directory found.\n");
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
// List what is 'around' the given directory
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
// Deciphers the command line in 
void process_command(char **line_in)
{
    if(line_in != NULL)
    {
        switch (commandType(line_in[0])) 
        {
            case CREATE:
                printf("User picked Create.\n"); 
                if(equalStrings(line_in[1], File))
                {
                    char *fileName = line_in[2]; 
                    add_File_To_Dir(head, fileName);
                    // add_file_to_dir
                }
                else if(equalStrings(line_in[1], Dir))
                {
                    printf("User picked directory.\n");
                    char *directoryName = line_in[2]; 
                    add_Dir(&head, directoryName); 
                }
                else
                {
                    printf("Invalid action.\n"); 
                }
                break; 
            case DELETE:
                printf("User picked Delete.\n"); 
                if(equalStrings(line_in[1], File))
                {
                    printf("User picked file.\n");
                    for(int i = 0; i < MAX_FILE_AMOUNT; i++)
                    {
                        if(equalStrings(line_in[2], head->files[i].fileName))
                        {
                            deleteFile(head->files[i].fileName);
                            printf("File '%s' deleted\n", line_in[2]);
                            break; 
                        }
                        if(i == (MAX_FILE_AMOUNT - 1))
                        {
                            printf("File '%s' not found.\n", line_in[2]); 
                        }
                    }
                    
                    
                    // look for file in directory and rm
                }
                else if(equalStrings(line_in[1], Dir))
                {
                    printf("User picked directory.\n"); 
                    // remove given directory 
                }
                else
                {
                    printf("Invalid action.\n"); 
                }
                break;
            case EDIT: 
                printf("User picked Edit.\n"); 
                if(equalStrings(line_in[1], File))
                {
                    printf("User picked file.\n"); 
                    // append given file in the directory
                }
                else if(equalStrings(line_in[1], Dir))
                {
                    printf("User picked directory.\n"); 
                    // rename directory? 
                }
                else
                {
                    printf("Invalid action.\n"); 
                }
                break;
            case SET:
                printf("User picked to set a directory\n"); 
                if(equalStrings(line_in[1], Dir))
                {
                    // TODO: use lookup_Dir and set it to a new dirNode pointer
                    // to preserve the original 'head'. Search using 'head'. 
                    // never change 'head' value. 
                    printf("Setting %s as a directory.\n", line_in[2]); 
                    // choosing an already defined directory
                }
                else
                {
                    printf("%s is not a directory.\n", line_in[2]); 
                }
                break; 
            case READ:
                printf("User picked Read.\n"); 
                if(equalStrings(line_in[1], File))
                {
                    printf("User picked file.\n"); 
                    // read file from a directory
                }
                else if(equalStrings(line_in[1], Dir))
                {
                    printf("User picked directory.\n"); 
                    // list what the directory has?
                }
                else
                {
                    printf("Invalid action.\n"); 
                }
                break;
            case HELP: 
                command_options(); 
                break; 
            case EXT:
                printf("Exiting...\n");
                running = false; 
                break; 
            default:
                printf("Choose something appropriate.\n"); 
                break;
        }
        //free(line_in); // Frees up the whole command in 
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



