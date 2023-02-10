// pingpong.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int pleft[2]){
    int p2c[2];
    pipe(p2c);

    if(fork()==0){

        
        int n;
        int pnext[2];
        pipe(pnext);
        while(read(p2c[0],&n,sizeof(n))>0){
            write(pnext[1],&n,sizeof(n));
        }
        close(p2c[0]);
        prime(pnext);
        close(pnext[1]);
        wait(0);
        exit(0);
    }else{
        int p;
        int n;
        read(pleft[0],&p,sizeof(p));
        printf("prime %d\n",p);
        while(read(pleft[0],&n,sizeof(n))>0){
            if(p%n!=0) write(p2c[1],&n,sizeof(n));
        }
        close(pleft[0]);
        close(p2c[1]);
        wait(0);
        exit(0);
    }

}

int main(int argc, char *argv[]) {
   int p1[2];
   pipe(p1);
   for(int i=2;i<=35;i++) write(p1[1],&i,sizeof(i));

    if(fork()==0){
        int pleft[2];
        pipe(pleft);
        int p,n;
        read(p1[0],&p,sizeof(p));
        printf("prime %d\n",p);
        while(read(p1[0],&n,sizeof(n))>0){
            write(pleft[1],&n,sizeof(n));
        }
        prime(pleft);
        wait(0);
        exit(0);
    }

    wait(0);
    
    exit(0);
}