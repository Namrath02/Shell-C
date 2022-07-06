#include"headerfiles.h"

void CtlC(int signum)
{
    if (foreground_running >0) //fg is running
    {
        kill(foreground_running, SIGINT);
        printf("\n");
        fflush(stdout);
    }
    else
    {
        printf("\n");
        prompt_generate();
        fflush(stdout);
    }
    return;
}

void CtrlZ(int signum){
    if(foreground_running>0) //fg is rnning
    {
        
        kill(foreground_running, SIGTSTP);
        bg_info_array[bg_index].pid=foreground_running;
        bg_info_array[bg_index].status=1;
        bg_info_array[bg_index].pid=fg_pid;
        strcpy(bg_info_array[bg_index].cmd,foreground_process_cmd);
        
        bg_index++;
        //prompt_generate();
        printf("\n");
        fflush(stdout);
    }
    else{
        printf("\n");
        fflush(stdout);
    }
    return ;
}
void childhandle(int signum){
    for(int i=0;i<1024;i++){
        if(bg_info_array[i].status==1){
            int status;
            int pid_val = bg_info_array[i].pid;
            int ret = waitpid(pid_val,&status,WNOHANG);
            if(ret>0){
                //status changed and exit nromally
                bg_info_array[i].status=0;
                if(status==0)
                printf("Process with PID %d exited Normally!\n",pid_val);
                else
                printf("Process with PID %d exited Abnormally!\n",pid_val);

                prompt_generate();
                fflush(stdout);
            } 
        }
    }
    //fflush(stdout);
    
    return;
}


void sig(char* line){
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
    if(i<=2) {printf("Insufficient Arguments!\n"); return;} 

    int job_nm = atoi(args[1]);
    int signum = atoi(args[2]);
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
    kill(pid,signum);
    return;
}