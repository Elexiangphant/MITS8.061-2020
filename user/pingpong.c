#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	
	
	if(fork() != 0) { 
		
		wait(0);
		printf("%d: received pong\n", getpid()); 
		
	} else { 
		
		printf("%d: received ping\n", getpid());
		
	}
	exit(0);
}