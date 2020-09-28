#include "stdio.h"
#include "unistd.h"

// 1. 当父进程先结束,子进程后结束,子进程被1号init进程领养,成为孤儿进程
//
// 2. 当孤儿进程死掉后,会回收自己的用户资源,但是无法回收自己的内核资源(需要父进程在结束时进行回收)
//
// 3. 当子进程先结束,父进程未结束,父进程并没有回收子进程的内核资源,子进程成为僵尸进程
//
// 4. 僵尸进程无法被kill -9杀死,因为它已经无法接受信号了

int main() {
    int pid = fork();

    if (pid == 0) {
        printf("child exit ...\n");
        return 0;
    }


    sleep(100);

    // use ps -ef | grep defunct watch

    return 0;
}