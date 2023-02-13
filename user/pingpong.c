#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	int pp[2],pc[2];
	
	pipe(pp);
	pipe(pc);
	
	if(fork() != 0) { 
		char buf;
		write(pp[1],"!",1);
		read(pc[0],&buf,1);
		printf("%d: received pong\n", getpid()); 
		wait(0);
		
	} else { 
		char buff;
		read(pp[0],&buff,1);
		
		printf("%d: received ping\n", getpid());
		write(pc[1],&buff,1);
		
	}
	exit(0);
}