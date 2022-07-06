#include "headerfiles.h"
//bg_pid[],bg_cmd[],bg_status[]

void prompt_generate(){
    char* cwd=(char*)malloc(1024);
    getcwd(cwd,1024);
    print_dir_name();
        printf("\033[0;34m");

        if(strcmp(cwd,home)==0)
        printf("~$ ");
        else{
            if(strlen(cwd)>strlen(home)){
                printf("~");
                for(int i=strlen(home);i<strlen(cwd);i++){
                    printf("%c",cwd[i]);
                }
                printf("$ ");
            }
            else{
                printf("%s~$",cwd);
            }
        }

        printf("\033[0m");
}

void print_dir_name(){
    char* hostname= (char*)malloc(sizeof(char)*1024);
    char* username = (char*)malloc(1024);
    gethostname(hostname, 1024);
    //hostname="LAPTOP123456";
    username=getenv("USER");
    if(username==NULL) return;
    printf("\033[0;32m");
    printf("%s@%s:",username,hostname);
    printf("\033[0m");

    return;
}

int get_number_slashes(char* path, int size){
    int result=0;
    for(int i=0;i<size;i++){
        if(path[i]=='/') result++;
    }
    return result;
}

int main(){
    //global var declarations
    bg_index=0;
    for(int i=0;i<1024;i++){
        bg_info_array[i].status=-1; bg_info_array[i].pid=0; 
        }
    exit_checker=0;
    foreground_running=false;

    char default_path[1024]; getcwd(default_path,sizeof(default_path));
    int default_slash_count= get_number_slashes(default_path,strlen(default_path));
    
    char* cwd=(char*)malloc(1024);
    getcwd(cwd,1024);
    getcwd(home,sizeof(home)); //current path, initially same as default but when cd it changes
    //from (def_slash_count+1)th slash everything will be same in rel. path

    signal(SIGCHLD,childhandle);
    signal(SIGINT,CtlC);
    signal(SIGTSTP,CtrlZ);
    while(1){
        
        print_dir_name();
        printf("\033[0;34m");

        if(strcmp(cwd,home)==0)
        printf("~$ ");
        else{
            if(strlen(cwd)>strlen(home)){
                printf("~");
                for(int i=strlen(home);i<strlen(cwd);i++){
                    printf("%c",cwd[i]);
                }
                printf("$ ");
            }
            else{
                printf("%s~$",cwd);
            }
        }

        printf("\033[0m");
        
        char* inp = (char*)malloc(1024);
        inp= parse_line(); //sanitised version
       
        if(exit_checker==1) break;
        if(strcmp(inp,"quit")==0 || strcmp(inp,"exit")==0) break; //
        
        command_read(inp,strlen(inp),&cwd);
		
        
        //break;
    }
    return 0;
}
