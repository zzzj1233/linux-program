#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "sys/wait.h"

// 1. 管道本质上是一块内核缓冲区
// 2. 管道分别对应两个文件描述符,一个读一个写
// 3. 管道的读写只能是单向的,如果要双向通信,则需要创建两个管道
// 4. 从管道中最大能存放的数据大小为4KB,使用ulimit -a命令可以查看,往管道放数据,读出来时,数据便会从管道中剔除
// 5. 管道只能适用于有血缘关系的进程通信

// int pipe(int fds[2])
//
// fds[0] = read
// fds[1] = write

int main(int argc, char **argv) {
    int fd[2] = {0};

    int ret = pipe(fd);

    if (ret < 0) {
        perror("open pipe error ");
    }

    int pid = fork();

    int read_fd = fd[0];
    int write_fd = fd[1];

    if (pid > 0) {

        // 父进程关闭读通道
        close(read_fd);

        // 写内容给子进程
        char *content = argc >= 2 ? argv[1] : "Hello World ~ My child \n";

        // \0也要写进去
        write(write_fd, content, strlen(content) + 1);

        // 等待子进程结束
        wait(NULL);

        printf("exit ... \n");
    } else {
        // 子进程关闭写通道
        close(write_fd);

        char buf[64];

        int len = read(read_fd, buf, 64);

        printf("child rend len = %d,content = %s", len, buf);
    }

}
