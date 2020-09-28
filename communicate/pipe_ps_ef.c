#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"

// 使用piep实现ps -ef功能

int main() {

    // 1. 拼接命令 --> ps - ef | grep getpid()
    char pid[10] = {0};
    sprintf(pid, "%d", getpid());
    char command[64] = {0};
    strcat(command, "ps -ef | grep ");
    strcat(command, pid);

    // 2. 创建通道
    int fds[2] = {0};

    pipe(fds);

    int readFd = fds[0];
    int writeFd = fds[0];

    // 3. 使用dup2切换通道
    dup2(STDOUT_FILENO, writeFd);

    // 4. fork子进程
    int childPid = fork();

    if (childPid > 0) {
        // 父进程关闭读通道
        close(readFd);

        sleep(1);

        // 执行命令,内容将被输出到写通道中
        system(command);

        // 等待子进程结束
        wait(NULL);
    } else {
        // 子进程关闭写通道
        close(writeFd);

        // 子进程读取内容
        // 1. 使用stdin去读
        dup2(STDIN_FILENO, readFd);

        printf("child prepared... \n");

        // 2. 手动读
        // char buf[1024] = {0};

        // read(readFd, buf, 1024);

        // printf("子进程读取到: %s \n", buf);
    }

}