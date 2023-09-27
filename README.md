# Shelly

Shelly is a custom command line interface (CLI) that is able to manipulate files and directories. It is not meant to replace an actual CLI. The purpose for this project was to challenge myself to create something similar to a CLI implemented in C. 

# How to use Shelly

When Shelly launches, it expects the user's input to be formatted in a single line with three components all separated by a space. The components are: a command, an object type, the object's name. You know Shelly is waiting for your command when you see `>`. If you are inside a directory, you will see `[name of directory]>`

## Commands

A command is the functionality that Shelly needs to do. Shelly will look for its command in the first word of the user's input. Commands are case senstive and they need to be typed in all lowercase letters, they include: create, set, delete, edit, read, ls, clear, copy, and exit. 

## Object Types

Shelly only understands two object types, files interpreted as 'file' and directories interpreted as 'dir'. Shelly will look for the object type in the second word of the user's input. 

## Object's Name

The last word in an input should be the name the user wishes to give to an object type (files or directories). 

## Details of each command

### The `create` command
To use this command make sure to type 'create' at the beginning of your input. This lets Shelly know you want to create either a file or a directory. It will then look for what you actually want it to create in the second part of the input. Then it'll assign the last word of the input (object's name) to the object you want to create. Example: `create dir my_directory`

### The `set` command
You are not able to create any files if you don't set what directory you wish to create those files in. The 'set' command is only used for setting your current directory. You can create as many directories as you want but you won't be able to do much else unless you set a directory first. This tells Shelly you wish you switch your current directory, and all the commmands after this will pertain only to this new directory. Keep in mind that you are not able to switch to a directory that is several directories deep from your current one. You have to traverse the directories until you reach the one you want. Example: `set dir my_directory`

### The `delete` command
Typing 'delete' at the beginning of your input will let Shelly know you want to delete something. This command is used just like 'create'. You provide the command 'delete' followed by the object type 'file' or 'dir' and finally the name of the object you want to delete. When applying to directories, Shelly will delete all the files inside that directory before deleting the directry itself. Example: `delete dir my_directory`

### The `edit` and `read` commands
'edit' will allow you to append a file or change the name of a directory. In both cases, you are able to type the new contents of a file or the new name of your directory after the command 'edit' gets interpreted. Example: `edit file data.txt`
'read' will allow you to read any file in your current directory, it'll display the contents of a file. Example: `read file data.txt`

### The `ls` command
'ls' lists the files and other directories of your current directory. This is a special command because you only need to type 'ls' to execute it; Shelly knows that 'ls' means to display the current directory and there's no need for any further information. Example: `ls`

### The `clear` command
The 'clear' command empties out the files of your current directory. This is useful when you want to delete all the files of your current directory in one go. Example: `clear dir my_directory`

### The `copy` command
`copy` is used when you want to copy the contents of one file and add it to another file. `copy` is another special command because it doesn't require an object type. Shelly knows that you are talking about files when you use `copy` so you only need to provide the two files. The first file is the file you wish to get the contents from, while the second file you provide is the file you wish to place the contents of the first one to. Example: `copy data.txt newdata.txt`

### The `exit` command
This command exits Shelly. All the files and directories that were not deleted prior to exiting will be deleted. 

### Some special functions
There are some exceptions to how Shelly interprets commands. Sometimes Shelly can break its own rules and only require one word to execute a command like with `ls` and `exit`, and sometimes it doesn't need all the three components for a command like with `copy`. The command `set` is used to set your current directory as in `set dir my_directory`, but it can also be used with `..` to quickly go up one directory like `set ..`. This is much faster than typing the 'dir' type and the name of the directory. 

## Features I want to add
Shelly may get changed to have it include better functionality. Here are some things I would like to add: 

For commands like `set`, `read`, and `clear`, where only apply to one object type (file or directory), I would like to make typing them, a little faster by removing the need to include the object type. So for example to read a file, instead of typing `read file data.txt` change it to `read data.txt` because `read` only works with files so what's the point of needing to specify the object type. 

Include something similar to `set ..` but in the other direction. Shelly (for now) only allows to nest directories, not create them side by side so having something similar to `set ..` but in the other way would be useful. 

