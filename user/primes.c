// pingpong.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int pleft[2]){
    int p;
	read(pleft[0], &p, sizeof(p)); // 读第一个数，必然是素数
	if(p == -1) { // 如果是哨兵 -1，则代表所有数字处理完毕，退出程序
		exit(0);
	}
	printf("prime %d\n", p);

	int pright[2];
	pipe(pright); // 创建用于输出到下一 stage 的进程的输出管道 pright

	if(fork() == 0) {
		// 子进程 （下一个 stage）
		close(pright[1]); // 子进程只需要对输入管道 pright 进行读，而不需要写，所以关掉子进程的输入管道写文件描述符，降低进程打开的文件描述符数量
		close(pleft[0]); // 这里的 pleft 是*父进程*的输入管道，子进程用不到，关掉
		prime(pright); // 子进程以父进程的输出管道作为输入，开始进行下一个 stage 的处理。

	} else {
		// 父进程 （当前 stage）
		close(pright[0]); // 同上，父进程只需要对子进程的输入管道进行写而不需要读，所以关掉父进程的读文件描述符
		int buf;
		while(read(pleft[0], &buf, sizeof(buf)) && buf != -1) { // 从左端的进程读入数字
			if(buf % p != 0) { // 筛掉能被该进程筛掉的数字
				write(pright[1], &buf, sizeof(buf)); // 将剩余的数字写到右端进程
			}
		}
		buf = -1;
		write(pright[1], &buf, sizeof(buf)); // 补写最后的 -1，标示输入完成。
		wait(0); // 等待该进程的子进程完成，也就是下一 stage
		exit(0);
	}
}

int main(int argc, char *argv[]) {
   int input_pipe[2];
	pipe(input_pipe); // 准备好输入管道，输入 2 到 35 之间的所有整数。

	if(fork() == 0) {
		// 第一个 stage 的子进程
		close(input_pipe[1]); // 子进程只需要读输入管道，而不需要写，关掉子进程的管道写文件描述符
		prime(input_pipe);
		exit(0);
	} else {
		// 主进程
		close(input_pipe[0]); // 同上
		int i;
		for(i=2;i<=35;i++){ // 生成 [2, 35]，输入管道链最左端
			write(input_pipe[1], &i, sizeof(i));
		}
		i = -1;
		write(input_pipe[1], &i, sizeof(i)); // 末尾输入 -1，用于标识输入完成
	}
	wait(0); // 等待第一个 stage 完成。注意：这里无法等待子进程的子进程，只能等待直接子进程，无法等待间接子进程。在 sieve() 中会为每个 stage 再各自执行 wait(0)，形成等待链。
	exit(0);

   
}