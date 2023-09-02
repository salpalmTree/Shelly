#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include "../include/shelly_func.h"

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