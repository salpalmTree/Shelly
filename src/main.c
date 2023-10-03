#include "../inc/shelly_func.h"
bool running;  
dirNode *head = NULL;
dirNode *set_head = NULL; 
char *userInput; 
char **parsed_command; 
int main(void)
{
    printf("Welcome to Shelly, a custom Command Line Interface/File manager.\n");   
    running = true;
    do {
        if(set_head) { printf("\t%s >", set_head->dirName); }
        else { printf("\t>"); }
        userInput = getInput(); 
        parsed_command = parse_command(userInput);
        if(!parsed_command[0]) { printf("\n");}
        else
        {
            switch (commandType(parsed_command[0])) 
            {
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
                    else if(equalStrings(parsed_command[1], "copy"))
                    {
                        if(!set_head)
                        {
                            printf("Directory not set.\n"); 
                        }
                        else
                        {
                            create_copy(set_head, parsed_command[2]); 
                        }
                    }
                    else
                    {
                        printf("Please choose something valid.\n"); 
                    }
                    break;
                case DELETE:
                    if(equalStrings(parsed_command[1], File))
                    {
                        if(!set_head)
                        {
                            printf("Directory not set.\n"); 
                        }
                        else
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
                    }
                    else if(equalStrings(parsed_command[1], Dir))
                    {
                        if(!set_head)
                        {
                            printf("Directory not set.\n"); 
                        }
                        else
                        {
                            rm_directory(set_head, parsed_command[2]); 
                        }
                    }
                    else
                    {
                        printf("Please choose something valid.\n"); 
                    }
                    break; 
                case EDIT:
                    if(equalStrings(parsed_command[1], File))
                    {
                        if(!set_head)
                        {
                            printf("Directory not set.\n"); 
                        }
                        else
                        {
                            printf("Editing file...\n");
                            int8_t valid_file = find_file(set_head, parsed_command[2]); 
                            if(valid_file >= 0)
                            {
                                editFile(set_head->files[valid_file].fileName);
                            }
                            else { printf("File '%s' not found.\n", parsed_command[2]); }
                        }
                    }
                    else if(equalStrings(parsed_command[1], Dir))
                    {
                        if(!set_head)
                        {
                            printf("Directory not set.\n"); 
                        }
                        else
                        {
                            printf("Editing dir name...\n"); 
                            printf("New name for '%s' directory.\n", set_head->dirName); 
                            set_head->dirName = getInput(); 
                        }
                    }
                    else
                    {
                        printf("Choose something valid.\n"); 
                    }
                    break; 
                case SET:
                    // the 'dir' in 'set dir mydir' is no longer needed
                    if(equalStrings(parsed_command[1], ".."))
                    {
                        set_head = back_one_directory(set_head); 
                    }
                    else if((set_head = lookup_Dir(head, parsed_command[1])))
                    {
                        printf("Directory '%s' set\n", set_head->dirName);  
                    }
                    else
                    {
                        printf("Directory doesn't exist.\n"); 
                    }
                    break; 
                case READ:
                    // 'file' in 'read file data.txt' no longer needed
                    if(!set_head)
                    {
                        printf("Directory not set.\n");
                    }
                    else
                    {
                        int8_t valid_file = find_file(set_head, parsed_command[1]); 
                        if(valid_file >= 0)
                        {
                            readFile(set_head->files[valid_file].fileName); 
                        }
                        else { printf("File '%s' not found.\n", parsed_command[1]); }
                    }
                    break; 
                case COPY: 
                    if(!set_head)
                    {
                        printf("Directory not set.\n");
                    }
                    else 
                    {
                        int first = find_file(set_head, parsed_command[1]); 
                        int second = find_file(set_head, parsed_command[2]); 
                        if(first >= 0 && second >= 0)
                        {
                            copy_file_file(set_head->files[first].fileName, set_head->files[second].fileName); 
                        }
                    }
                    break; 
                case CLEAR: 
                    // 'dir' in 'clear dir mydir' no longer needed
                    if(!set_head)
                    {
                        printf("Directory not set.\n");
                    }
                    else
                    {
                        rm_all_files(set_head); 
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
                    printf("Exiting...\n"); 
                    running = false; 
                    break; 
                default:
                    printf("Choose something valid.\n"); 
                    break; 
            }
        }
    }while(running == true);   

    clean_up(head, userInput, parsed_command);
    return 0; 
}
