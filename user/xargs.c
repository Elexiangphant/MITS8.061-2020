#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(char *com,char *arg[MAXARG]){

    if(fork()==0)exec(com,arg);
    else wait(0);
    exit(0);
    

}

int main(int argc,char *argv[]){

    if(argc<2) exit(0);

    //int pid,status;
    //pid = fork();
    
        char com[10];
        strcpy(com,argv[1]);
        char *varg[MAXARG];
        char **pvarg=varg;
        int index=1;
        for(int i=1;i<argc;i++){
            *pvarg = argv[i];
            pvarg++;
            index++;
        }
        
        int prime=index;
        char arg[MAXARG];
        while(read(0,arg,sizeof(arg))>0){
            char argvv[MAXARG];
            int n=0;
            for(int i=0;i<sizeof(arg);i++){
                if(arg[i]=='\n'||' '){
                    argvv[n]='\0';
                    strcpy(varg[index++],argvv);
                    if(arg[i]=='\n'){
                    
                    index=prime;
                    n=0;
                    xargs(com,varg);
                    }
                }else{
                    argvv[n++]=arg[i];
                }
            }
            argvv[n]='\0';
            strcpy(varg[index++],argvv);
            xargs(com,varg);
        }
        while(wait(0)!=-1){}
    exit(0);

}