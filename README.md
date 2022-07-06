## OS Assignment 2 - Shell in C:
 Roll Number: 2020101063  

## BUILTIN COMMANDS SUPPORTED

* exit
* quit
* echo
* pwd
* cd
* ls


## FILES USED

| FILE NAME                  | DESCRIPTION                                                                                                                                      |
|----------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| Makefile                   | contains all the instructions required to make and run the shell.                                                                                |
| headerfiles.h                    | contains the list of all header files, constants, global variables, and function prototypes                                                      |
| main.c                     | contains the main function which runs the shell                                                                                                  |
| parse.c  | Parsing the input through Command-Line                               |
| commands.c    | Contains the functions for builtin commands and allotting for them  
|pipes.c   | Contains the piping across commands and related functions.
| signal_handling.c | Contains all the functions dealing with signal handling like CtrlC , CtrlZ and CtrlD.        
| jobs.c | Contains the function to run jobs command to view processes in background.                                         