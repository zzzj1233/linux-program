#include "unistd.h"
#include "sys/wait.h"
#include "stdio.h"
#include "fcntl.h"

/**
 * 1. 使用fcntl函数将文件描述符设置为非阻塞
 * 
 * 2. int read()
 *    > 0 = 读到的字节数
 *    - 1 = 未读到数据
 *      0 = 写端已经关闭
 */

int main() {
    // 1. 创建管道
    int fds[2] = {0};
    pipe(fds);

    int readFd = fds[0];
    int writeFd = fds[1];

    // 2. fork子进程
    int pid = fork();

    if (pid == 0) {
        close(readFd);

        // 休眠3s给子进程传送数据
        sleep(3);

        printf("父进程休眠3s结束\n");

        write(writeFd, "Hello World", 12);

        // 关闭通道
        close(writeFd);

        // 等待子进程结束
        wait(NULL);

    } else {
        close(writeFd);

        // 使用fcntl设置非阻塞
        int flag = fcntl(readFd, F_GETFL);

        flag |= O_NONBLOCK;

        fcntl(readFd, F_SETFL, flag);

        // 使用非阻塞读
        char buf[64];

        while (1) {
            int ret = read(readFd, buf, 64);

            if (ret == -1) {
                printf("未读到数据,将休眠1s \n");
                sleep(1);
            } else if (ret == 0) {
                printf("写端已关闭 即将退出程序\n");
                break;
            } else {
                printf("读到数据为 : %s \n", buf);
            }
        }

    }


}

