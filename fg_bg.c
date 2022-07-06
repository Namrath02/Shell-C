#include"headerfiles.h"

void bg(char* line){
    int i=0; char* args[100];
    char* token=(char*)malloc(1024);
    char* ln = (char*)malloc(1024);
    strcpy(ln,line);
    while(1){
        token=strtok(ln," "); //kill 2 9
        if(token==NULL) break;
        args[i]=token;
        
        if((strcmp(args[i],"<")==0)||(strcmp(args[i],">>")==0) || (strcmp(args[i],">")==0)) 
            args[i]=NULL;
        i++;
        ln = NULL;
    }
    args[i]=NULL;
    if(i<=1) {printf("Insufficient Arguments!\n"); return;} 

    int job_nm = atoi(args[1]);
    int pid=-1;
    int index=0;
    for(int k=0;k<1024;k++){
        if(bg_info_array[k].status==1){
            if(index==(job_nm-1)) pid=bg_info_array[k].pid;
            else index++;
        }
    }
    if(pid==-1) printf("Invalid Job Number Provided!\n");

    kill(pid,SIGCONT);
    return;
}

void fg(char* line){
    int status;
    int i=0; char* args[100];
    char* token=(char*)malloc(1024);
    char* ln = (char*)malloc(1024);
    strcpy(ln,line);
    while(1){
        token=strtok(ln," "); //kill 2 9
        if(token==NULL) break;
        args[i]=token;
        
        if((strcmp(args[i],"<")==0)||(strcmp(args[i],">>")==0) || (strcmp(args[i],">")==0)) 
            args[i]=NULL;
        i++;
        ln = NULL;
    }
    args[i]=NULL;
    if(i<=1) {printf("Insufficient Arguments!\n"); return;} 

    int job_nm = atoi(args[1]);
    int pid=-1;
    int index=0;
    for(int k=0;k<1024;k++){
        if(bg_info_array[k].status==1){
            if(index==(job_nm-1)) pid=bg_info_array[k].pid;
            else index++;
        }
    }
    if(pid==-1) printf("Invalid Job Number Provided!\n");
    else 
    waitpid(pid,&status,WUNTRACED);

    return;
}