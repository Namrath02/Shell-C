#include"headerfiles.h"
#include<signal.h>

void exec_echo(char* input, int t_size){
    //seperate the echo out
   
    char* print_part = (char*)malloc(1024*sizeof(char));
    for(int i = t_size+1; i < strlen(input) && input[i] != '<' && input[i] != '>'; i++)
    print_part[i-t_size-1]=input[i];
    print_part[strlen(input)-1-t_size]='\0';
    printf("%s\n",print_part);
    free(print_part);
    return;
}

void exec_cd(char* input, int t_size,char** cwd){
    //check if its just "cd" or "cd <something>"
    if(strlen(input)==t_size) {chdir(home);int len = strlen(home);
        for(int i = 0; i < len; i++)*cwd[i]=home[i];} 
    else { //seperate the cd out
        int siz = strlen(input); char* dir = (char*)malloc(1024);
        for(int i=t_size+1;i<siz;i++) dir[i-t_size-1]=input[i];
        dir[siz-t_size-1]='\0';

        int ret = chdir(dir);
        if(ret==-1) {perror("Error");return;}
        free(dir);
        getcwd(*cwd,1024);
    }
    return;
}

void repeat_interpreter(char* cmd,int times,char** cwd){
   int len = strlen(cmd);
   char* tok = (char*)malloc(1024);
   int whitespace = -1;
   for(int i=0;i<len;i++){
   if(cmd[i]==' ') {whitespace=i;break;}
   else tok[i]=cmd[i];
   }

   if(whitespace!=-1)
   tok[whitespace]='\0';
   else{
       tok[len]='\0';
   }
   char* commd = (char*)malloc(1024);
   for(int i=whitespace+1;i<len;i++)
   commd[i-whitespace-1]=cmd[i];

   commd[len-whitespace-1]='\0';
   

   if(strcmp(tok,"echo")==0){
            for(int k=0;k<times;k++)
            exec_echo(cmd,strlen(tok)); //pass the command directly
        }
        else if(strcmp(tok,"pwd")==0){
            char* pwdval = (char*)malloc(1024*sizeof(char));
            getcwd(pwdval,1024);

             for(int k=0;k<times;k++)
             printf("%s\n",pwdval);

            free(pwdval); //free up memory
        }
        else if(strcmp(tok,"cd")==0){
            for(int k=0;k<times;k++)
            exec_cd(cmd,strlen(tok),cwd);
        } 
        else{
            perror("unexpected command");
        }
   return;
   
}

int redirect(char* line){
    //cmd[i] is command of form cat < a.txt
    int i=0; char* args[100];
    char* token= (char*)malloc(1024);
    char* ln = (char*)malloc(1024);
    strcpy(ln,line);
   // printf("strtok loop reached\n");
    while(1){
        token= strtok(ln," ");
        if(token==NULL) break;
        args[i]=token;
        i++;
        ln = NULL;
    }
    args[i]= NULL;
    int op=0,append=0,read=0;
   // printf("strcmp loop reached\n");
    for(int j=0;j<i;j++)
    {
        if(!strcmp(args[j],">")) op=j;
        else if(!strcmp(args[j],">>")) append=j;
        else if(!strcmp(args[j],"<")) read=j;
    }
    //printf("dup reached values are %d %d %d\n",op,append,read);
    if(op>0){
        //filename= args[op+1]
        char* filename = (char*)malloc(1024);
        filename = args[op+1];
        int filedesc = open(filename,O_CREAT | O_WRONLY | O_TRUNC,0644);
        output_fd = dup(1); //0 1 
        dup2(filedesc,1);
        //free(filename);
    }
    if(append>0){
        //filename = args[append+1]
        char* filename = (char*)malloc(1024);
        filename= args[append+1];
        int filedesc= open(filename,O_CREAT | O_WRONLY | O_APPEND,0644);
        output_fd= dup(1);
        dup2(filedesc,1);
        //free(filename);
    }
    if(read>0){
        char* filename = (char*)malloc(1024);
        filename= args[read+1];
        int filedesc; 
        if((filedesc = open(filename,O_RDONLY))==-1) perror("Error!");
        input_fd = dup(0);
        dup2(filedesc,0);
       // free(filename);
    }
    if((op>0 || append>0) && read>0) return 3;
    else if(read>0) return 1;
    else if((op>0)||(append>0)) return 2;
    else return 0;
}

void default_redirect(int token){
    if(token==3){
        dup2(input_fd,0);
        dup2(output_fd,1);
    }
    if(token==2){
        dup2(output_fd,1);
    }
    if(token==1){
        dup2(input_fd,0);
    }
    return;
}

void exec_running(char* line,int tok){
    int status;
   // printf("execing %s\n",line);
    int i=0; char* args[100];
        char* token= (char*)malloc(1024);
        char* ln = (char*)malloc(1024);
        strcpy(ln,line);
        while(1){
            token = strtok(ln," ");
            if(token==NULL) break;
            args[i]=token;
            if((strcmp(args[i],"<")==0)||(strcmp(args[i],">>")==0) || (strcmp(args[i],">")==0)) 
            args[i]=NULL;
            i++; 
            ln = NULL;
        }
        args[i]=NULL;
    
    strcpy(foreground_process_cmd,line);
    int forkret=fork();
    fg_pid=forkret;
    if(forkret==-1) 
    {
        perror("Unable to fork!"); exit(-1);
    }
    if(forkret==0){
        if(tok==0){//fg
            
            if(execvp(args[0],args)==-1){
                perror("Error in Foreground Process!"); exit(-1);
            }
            
        }
        else{//bg
            setpgid(0,0);
            if(execvp(args[0],args)==-1) {
                perror("Error in Background Process!"); exit(-1);
            }
        }
    }
    if(forkret>0){//parent
        if(tok==0){ //fg
            //printf("Reached wait statement of %s exec\n",line);
            foreground_running=true;
            waitpid(forkret,&status,WUNTRACED); //fg
            foreground_running=false;
            
            
        }
        else{ //bg
            //store_list(forkret,cmd[i],1);
            //printf("Ok ");
            printf("%d\n",forkret);
            kill(forkret,SIGCONT);
            bg_info_array[bg_index].pid=forkret;
            bg_info_array[bg_index].status=1;
            //bg_pid[bg_index]=forkret;
            //bg_status[bg_index]=1; //running
           
            strcpy(bg_info_array[bg_index].cmd,line); //store command
            bg_index++;
        }
    }
    free(token);
   // printf("Reached return statement of %s exec\n",line);
    return;
}

void command_read(char* input, int size,char** cwd){
    //first checking for ';' and '&'
    int count=0;
    for(int i = 0; i < size;i++){
        if(input[i] == ';') count++;
    }
    //number of commands would be count+1
    char cmd[count+1][100]; int j=0,k=0;
    char tokens[count+1][100];
    for(int i = 0; i <size; i++){
        if(input[i] != ';') {cmd[j][k]=input[i];k++;}
        else {cmd[j][k]='\0';j++;k=0;}
    }
    cmd[j][k]='\0';
  
    for(int i=0;i<count+1;i++){
        int j=0; 
        while(cmd[i][j] != ' ' && cmd[i][j] != '\0'){
            tokens[i][j]=cmd[i][j];j++;
        }
        tokens[i][j]='\0';
        
        //tokens is they function keyword and cmd is the command passed.
    }
    
    //we check for redirects before doing the func call and reset after it ends
    for(int i=0;i<count+1;i++){
        int pipe_tok=0;
        for(int k=0;k<strlen(cmd[i]);k++){
            if(cmd[i][k]=='|') pipe_tok++;
        }

        if(pipe_tok>0) pipe_exec(cmd[i],pipe_tok);
        else{
            int redirect_token = redirect(cmd[i]); //3 if both rw 2 if w 1 if r
            //printf("token is %s and cmd is %s\n",tokens[i],cmd[i]);
            if(strcmp(tokens[i],"echo")==0){
                exec_echo(cmd[i],strlen(tokens[i])); //pass the command directly
            }
            else if(strcmp(tokens[i],"pwd")==0){
                char* pwdval = (char*)malloc(1024*sizeof(char));
                getcwd(pwdval,1024); printf("%s\n",pwdval);
                free(pwdval); //free up memory
            }
            else if(strcmp(tokens[i],"cd")==0){
                exec_cd(cmd[i],strlen(tokens[i]),cwd);
            }
            else if(strcmp(tokens[i],"repeat")==0){//repeat x cmd someth
                char* rpt = (char*)malloc(1024);
                int times = cmd[i][7]-48; //x value ,as we know its sanitised alr
                for(int j=9;j<strlen(cmd[i]);j++)
                rpt[j-9]=cmd[i][j];

                rpt[strlen(cmd[i])-9]='\0';
                repeat_interpreter(rpt,times,cwd);
            }
            else if(strcmp(tokens[i],"jobs")==0){
                //printf("You called %s command",cmd[i]);
                exec_jobs(tokens[i],cmd[i]);
            }
            else if(strcmp(tokens[i],"sig")==0){
                sig(cmd[i]);
            }
            else if(strcmp(tokens[i],"bg")==0)
            {
                bg(cmd[i]);
            }
            else if(strcmp(tokens[i],"fg")==0){
                fg(cmd[i]);
            }
            else{
                int lgt = strlen(cmd[i]);
                int and_token=0;
                for(int k=0;k<lgt;k++) if(cmd[i][k]=='&') {and_token = 1;cmd[i][k]=' ';}
                //printf("%s andtoken is %d\n",cmd[i],and_token);
            
                exec_running(cmd[i],and_token);
            }
            default_redirect(redirect_token);
        }
    }
   return;
}

