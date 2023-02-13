#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(char *com,char *arg[MAXARG]){

    if(fork()==0){
        exec(com,arg);
        exit(0);
    }
    return;

}

int main(int argc,char *argv[]){

    if(argc<2) exit(0);

    //int pid,status;
    //pid = fork();
    
        char com[10];
        strcpy(com,argv[1]);
        char *varg[MAXARG];
        char **pvarg=varg;
        char buf[2048];
        char *p=buf,*last_p=buf;        
        for(int i=1;i<argc;i++){
            *pvarg = argv[i];
            pvarg++;
        }
        
        char **pa=pvarg;
        while(read(0,p,1)!=0){
            
            if(*p==' '||*p=='\n'){
                *p='\0';
                *(pa++)=last_p;
                last_p=p+1;
                if(*p=='\n'){
                    *pa=0;
                    xargs(com,varg);
                    pa=pvarg;
                }
            }
            p++;
        }
        if(pa!=pvarg){
            *p='\0';
            *(pa++)=last_p;
            *pa=0;
            xargs(com,varg);
        }
        while(wait(0)!=-1){}
    exit(0);

}