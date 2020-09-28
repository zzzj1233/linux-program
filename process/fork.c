#include "unistd.h"
#include "stdio.h"

// 1. 此时linux会为子进程复制一块完全相同的内存区域,包含用户区和内核区(仅PID不同)
// 2. 父进程fork函数返回子进程的PID
// 3. 子进程fork函数返回0(用于区分),看似会返回两个pid,其实是因为被复制了,两个进程在执行同一个程序
// 4. 如果父进程先执行完了,那么子进程的PPID将变成1,成为孤儿进程

int main() {

    int pid = fork();

    if (pid < 0) {
        perror("fork error");
        return -1;
    }

    // parent
    if (pid > 0) {

        printf("Parent --->  childPid = %d, currentId = %d, parentId = %d \n", pid, getpid(), getppid());
        sleep(1);
        // child
    } else {
        printf("Child --->  childPid = %d, currentId = %d, parentId = %d \n", pid, getpid(), getppid());
    }

    printf("common code ~ currentPId = %d \n",getpid());
}