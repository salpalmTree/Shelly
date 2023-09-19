#include "../include/shelly_func.h"

// TODO: add full functionality
//      add copy function [copy file -> file2]
//         and [create copy file.txt]
//      add 'delete all' for directory

bool running;  
dirNode *head = NULL;
dirNode *set_head = NULL; 
char *userInput; 
char **parsed_command; 
int main(void)
{
    printf("Welcome to Shelly... a super basic file shell thing.\n");   
    running = true;
    do {
        if(set_head) { printf("\t%s >", set_head->dirName); }
        else { printf("\t>"); }
        userInput = getInput(); 
        parsed_command = parse_command(userInput);
        switch (commandType(parsed_command[0])) {
            case CREATE:
                if(equalStrings(parsed_command[1], File))
                {
                    if(!set_head)
                    {
                        printf("Directory not set.\n"); 
                    }
                    else
                    {
                        add_File_To_Dir(set_head, parsed_command[2]);
                    }
                }
                else if(equalStrings(parsed_command[1], Dir))
                {
                    add_Dir(&head, parsed_command[2]); 
                }
                else
                {
                    printf("Please choose something valid.\n"); 
                }
                break; 
            case DELETE:
                if(equalStrings(parsed_command[1], File))
                {
                    int8_t valid_file = find_file(set_head, parsed_command[2]);
                    if(valid_file >= 0) 
                    {
                         printf("File '%s' has been deleted.\n", parsed_command[2]); 
                         deleteFile(parsed_command[2]);
                         set_head->files[valid_file].fileName = EMPTY_FILE; 
                    }
                    else { printf("File '%s' not found.\n", parsed_command[2]); }
                }
                else if(equalStrings(parsed_command[1], Dir))
                {
                    // rm directory
                }
                else
                {
                    printf("Please choose something valid.\n"); 
                }
                break;
            case EDIT:
                if(equalStrings(parsed_command[1], File))
                {
                    printf("Editing file...\n");
                    int8_t valid_file = find_file(set_head, parsed_command[2]); 
                    if(valid_file >= 0)
                    {
                        editFile(set_head->files[valid_file].fileName);
                    }
                    else { printf("File '%s' not found.\n", parsed_command[2]); }
                }
                else if(equalStrings(parsed_command[1], Dir))
                {
                    printf("Editing dir name...\n"); 
                }
                else
                {
                    printf("Choose something valid.\n"); 
                }
                break; 
            case SET:
                if(equalStrings(parsed_command[1], Dir))
                {
                    if(!(set_head = lookup_Dir(head, parsed_command[2])))
                    {
                        printf("Directory doesn't exist.\n"); 
                    }
                    else
                    {
                        printf("Directory '%s' set\n", set_head->dirName);  
                    }
                }
                else if(equalStrings(parsed_command[1], ".."))
                {
                    set_head = back_one_directory(set_head); 
                }
                break; 
            case READ:
                if(equalStrings(parsed_command[1], File))
                {
                    int8_t valid_file = find_file(set_head, parsed_command[2]); 
                    if(valid_file >= 0)
                    {
                        readFile(set_head->files[valid_file].fileName); 
                    }
                    else { printf("File '%s' not found.\n", parsed_command[2]); }
                }
                else
                {
                    printf("Choose something valid.\n"); 
                }
                break; 
            case LS:
                ls_Dir(set_head);
                //printDir(head); 
                break; 
            case HELP:
                command_options();  
                break; 
            case EXT:
                printf("Exiting..."); 
                running = false; 
                break; 
            default: 
                printf("Choose something valid.\n"); 
                break; 
        }
    }while (running == true); 

    free(head); 
    free(userInput); 
    free(parsed_command); 
    return 0; 
}
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
void deleteFile(char *fileName)
{
    if(remove(fileName))
    {
        printf("Could not delete that file.\n"); 
    }
    //free(fileName); 
}
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
        printf("\n"); 
        //free(fileName); 
        fclose(fileToRead); 
    }
}
void editFile(char *fileName)
{
    FILE *fileToAppend; 
    if((fileToAppend = fopen(fileName, "a")) == NULL)
    {
        printf("That file could not be edited.\n");
        //free(fileName); 
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
    }
}
void command_options(void)
{
    printf("Enter a [command] [obj. Type] [obj. Name]\n");
    printf("Commands can include:\n");
    printf("\tcreate, delete, edit, set, ls, and exit\n"); 
    printf("Obj. Types are:\n"); 
    printf("\t'file', and 'dir'\n");  
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
                bool file_deleted = false; 
                if(equalStrings(line_in[1], File))
                {
                    printf("User picked file.\n");
                    for(int i = 0; i < MAX_FILE_AMOUNT; i++)
                    {
                        if(equalStrings(line_in[2], head->files[i].fileName))
                        {
                            deleteFile(head->files[i].fileName);
                            printf("File '%s' deleted\n", line_in[2]);
                            head->files[i].fileName = EMPTY_FILE;
                            file_deleted = true; 
                            break; 
                        }
                        if(!file_deleted)
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
                else
                {
                    printf("Invalid action.\n"); 
                }
                break;
            case HELP: 
                command_options(); 
                break;
            case LS: 
                ls_Dir(head); 
                break ;
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



