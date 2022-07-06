#include"headerfiles.h"

char* parse_line(){
    char* input = (char*)malloc(sizeof(char)*1024);
    char* output = (char*)malloc(sizeof(char)*1024);
    char* buffer_res = (char*)malloc(sizeof(char)*1024);
    if(fgets(input,1024,stdin)==NULL) {exit_checker=1;return 0;}
    
    int length = strlen(input);
    int end=0,counter=0;
    int siz = strlen(input);
    for(int i = 0; i <siz;i++){
        if(input[i]=='\n' || input[i]=='\0') {end=counter;break;}
        else if(input[i]=='\t') {output[counter]=' ';counter++;}
        else if(input[i]==' ' && ((i+1)<(length-1))){
                if(input[i+1]==' ' || i==0) continue;
                else {output[counter]=' ';counter++;}
            }
        else{
            output[counter]=input[i];counter++;
        } 
    }
   
    output[end]='\0'; 
    int counter2=0; bool punc=false;
    for(int i=0;i<end;i++){
        if(output[i]==';'){punc=true; buffer_res[counter2]=output[i];counter2++;}
        else if(punc && output[i]==' ') continue;
        else{
            buffer_res[counter2]=output[i];counter2++; punc=false;
        }
    }
    buffer_res[counter2]='\0';
    free(input); //free up memory
    free(output);
    return buffer_res;
}
