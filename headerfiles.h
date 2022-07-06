#ifndef SHELL_H
#define SHELL_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include <limits.h>
#include <fcntl.h>
#include<signal.h>
#include<stdbool.h>
//#include<sys/wait.h>
//extern cwd[1024]
//extern home[1024]

char home[1024];
char history_path[1024];
//char* cwd;
int exit_checker;

int bg_index;
bool foreground_running;
char foreground_process_cmd[1024];
int fg_pid;
struct bg_info{
    int pid;
    char cmd[1024];
    int status;
};
struct bg_info bg_info_array[1024];

struct job_info{
    int pid;
    char cmd[1024];
    char status[1024];
    int index;
};


#define MAX_SIZE 1024
int output_fd;
int input_fd;
//LIST OF ALL FUNCTIONS USED
void print_dir_name();
int get_number_slashes(char* path, int size);
char* parse_line();
void command_read(char* input, int size,char** cwd);


void exec_cd(char* input, int t_size,char** cwd);
void exec_echo(char* input, int t_size);
void exec_running(char* line,int tok);

//piping
void pipe_exec(char* line,int tok);

//redirction
void default_redirect(int token);
int redirect(char* line);

//signal handlers
void CtlC(int signum);
void CtrlZ(int signum);
void childhandle(int signum);
void sig(char* line);

//user def
void exec_jobs(char*keyword,char* line);
void bg(char* line);
void fg(char* line);
void prompt_generate();

#endif