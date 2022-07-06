#include"headerfiles.h"

void exec_jobs(char*keyword,char* line){

    int status;
    struct job_info job_array[1024];
    int i=0; char* args[100];
    char* token=(char*)malloc(1024);
    char* ln = (char*)malloc(1024);
    strcpy(ln,line);
    while(1){
        token=strtok(ln," "); //jobs -r
        if(token==NULL) break;
        args[i]=token;
        if((strcmp(args[i],"<")==0)||(strcmp(args[i],">>")==0) || (strcmp(args[i],">")==0)) 
            args[i]=NULL;
        i++;
        ln = NULL;
    }
    args[i]=NULL;
    int r=0,s=0;
    int jk=0;
  
    while(args[jk]!=NULL){
        if(strcmp(args[jk],"-r")==0) r++;
        else if(strcmp(args[jk],"-s")==0) s++;
        jk++;
    }
    int ind=0;
   
        for(int i=0;i<1024;i++){
            if(bg_info_array[i].status==1){
                char* staticptr;
                int pidval = bg_info_array[i].pid;
                char *ln = (char*)malloc(1024); 
                for(int j=0;j<1024;j++) ln[i] = '\0';


                char* filename = (char*)malloc(1024); 
                for(int j=0;j<1024;j++) filename[j] = '\0';
                sprintf(filename,"/proc/%d/stat",pidval);


                FILE *f = fopen(filename,"r");
                if(f==NULL) printf("Error opening Stats File!\n");

                fgets(ln,10000,f);
                char *state = (char*)malloc(1024); 
                for(int jk=0;jk<1024;jk++) state[jk]='\0';
                int iter=0;
              
                while(1){
                    char *token = (char*)malloc(1024); 
                    token = strtok_r(ln," \t",&staticptr);
                    ln = NULL;
                    if(token == NULL)
                        break;
                    if(iter == 2) 
                    { 
                        state = token;
                        if((strcmp(state,"R")==0)||(strcmp(state,"S")==0)|| (strcmp(state,"D")==0))
                        strcpy(state,"Running");
                        else
                        strcpy(state,"Stopped");
                         break;
                    }
                    iter++;
                }
                    
                    job_array[ind].pid=pidval;
                    job_array[ind].index=i;
                    
                    strcpy(job_array[ind].cmd,bg_info_array[i].cmd);
                    strcpy(job_array[ind].status,state);
                    ind++;
                   
            } //if end
            
        }//for loop end
       //printf("Values of i and ind are %d %d\n",i,ind);
        for(int k=0;k<ind;k++)
            for(int j=k+1;j<ind;j++){
                if(strcmp(job_array[k].cmd,job_array[j].cmd)>0){
                    int temp_pid; int temp_index; char temp_cmd[1024]; char temp_status[1024];
                    temp_pid=job_array[k].pid;
                    temp_index=job_array[k].index;
                
                    strcpy(temp_cmd,job_array[k].cmd);
                    strcpy(job_array[k].cmd,job_array[j].cmd);
                    strcpy(job_array[j].cmd,temp_cmd);
                    
                    strcpy(temp_status,job_array[k].status);
                    strcpy(job_array[k].status,job_array[j].status);
                    strcpy(job_array[j].status,temp_status);

                    job_array[k].pid=job_array[j].pid;
                    job_array[j].pid=temp_pid;

                    job_array[k].index=job_array[j].index;
                    job_array[j].index=temp_index;
            }
        }

       
        for(int i=0;i<ind;i++){
            if(r>0)
            {
                if(strcmp(job_array[i].status,"Running")==0)  
                printf("[%d] %s %s [%d]\n",job_array[i].index+1,job_array[i].status,job_array[i].cmd,job_array[i].pid);
            }
            else if(s>0)
            {
                 if(strcmp(job_array[i].status,"Stopped")==0)  
                printf("[%d] %s %s [%d]\n",job_array[i].index+1,job_array[i].status,job_array[i].cmd,job_array[i].pid);
            }
            else{
                printf("[%d] %s %s [%d]\n",job_array[i].index+1,job_array[i].status,job_array[i].cmd,job_array[i].pid);
            }
           
        }
    return;
}

