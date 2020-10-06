#include "netinet/in.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"

// 多线程bio服务器

int while_condition = 1;

int fd = 0;

void cb(int sig) {
    while (1) {

        int ret = waitpid(-1, NULL, WNOHANG);

        printf("cb : ret = %d \n", ret);

        // 没有回收到
        if (ret == 0) {
            break;
        }

        // 回收到了
        if (ret > 0) {
            while_condition = 0;
            continue;
        }

        // 没有回收到
        if (ret == -1) {
            break;
        }

    }
}

void close_fd_cb(int sig) {
    close(fd);
    printf("server closed...");

    wait(NULL);
}

int main() {
    // 1. 创建socket

    fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定端口
    struct sockaddr_in sock;
    memset(&sock, 0x00, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(8082);
    sock.sin_addr.s_addr = INADDR_ANY;
    socklen_t size = sizeof(sock);

    bind(fd, (const struct sockaddr *) &sock, size);

    // 注册信号监听事件
    signal(SIGCHLD, cb);
    signal(SIGINT, close_fd_cb);

    // 3. 监听
    listen(fd, 128);

    while (while_condition) {
        struct sockaddr_in client_sock;
        memset(&sock, 0x00, sizeof(client_sock));
        socklen_t client_size = sizeof(client_size);

        int client = accept(fd, (struct sockaddr *) &client_sock, &client_size);

        if (client == -1) {
            perror("建立链接失败");
            return -1;
        }

        int pid = fork();

        if (pid == 0) {
            // 子进程关闭监听文件描述符
            close(fd);

            // 读写操作
            while (1) {
                char buf[1024] = {0};

                int read_ret = read(client, buf, sizeof(buf));

                if (read_ret == -1) {
                    perror("读取内容失败 ");
                    return -1;
                }

                if (read_ret == 0) {
                    printf("pid = %d 服务的客户端已经关闭 \n", getpid());
                    close(client);
                    return -1;
                }

                char send_buf[1024] = {0};

                sprintf(send_buf, "pid : %d , replay : %s \n", getpid(), buf);

                write(client, send_buf, sizeof(send_buf));
            }

        } else {
            // 父进程关闭读写文件描述符
            close(client);
        }
    }
    close(fd);

}
