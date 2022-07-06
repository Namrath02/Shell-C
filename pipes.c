#include "headerfiles.h"

void pipe_exec(char* line,int tok){ //tok = pipe count
    int fds[tok+2][2]; //file desc
    int i=0;
    char* ln = (char*)malloc(1024);
    char* token = (char*)malloc(1024);
    char* args[100];
    strcpy(ln,line);
    while (1)
    {
        token =strtok(ln,"|"); //ls | grep *.txt > out.txt; echo hello > input.txt | wc -l
        if(token==NULL) break;
        args[i]=token;
        i++; ln=NULL;
    }
    //args[i] is individual commands piped together
    for(int j=0;j<=tok;j++){
        if (pipe(fds[j]) < 0)
        {
            printf("Error creating Virtual File for Piping!\n");
        }
        if(j==0){
            int Outputfile = dup(1);
            dup2(fds[j][1], 1);
            close(fds[j][1]);
            int redirec_tok = redirect(args[j]);
            exec_running(args[j],0);
            dup2(Outputfile, 1);
            close(Outputfile);
            default_redirect(redirec_tok);
        }
        else if(j!=tok){
            int inputfile = dup(0);
            int outputfile = dup(1);
            dup2(fds[j][1], 1);
            dup2(fds[j - 1][0], 0);
            close(fds[j][1]);
            close(fds[j - 1][0]);

           int redirec_tok = redirect(args[j]);
            exec_running(args[j],0);
            dup2(inputfile, STDIN_FILENO);
            dup2(outputfile, STDOUT_FILENO);
            close(inputfile);
            close(outputfile);
            default_redirect(redirec_tok);
        }
        else{
            int inputfile = dup(0);
            dup2(fds[j - 1][0], 0);
            close(fds[j - 1][0]);
            int redirec_tok = redirect(args[j]);
            exec_running(args[j],0);
            dup2(inputfile, 0);
            close(inputfile);
            default_redirect(redirec_tok);
        }
    }
    return;
}