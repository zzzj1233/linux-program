#include "arpa/inet.h"
#include "sys/select.h"
#include "string.h"
#include "stdio.h"
#include "unistd.h"
#include "signal.h"

/**
 *
 * @param nfds      三个文件描述符队列最大文件描述符 + 1,因为文件描述符从0开始,内核遍历时的终止值
 * @param readfds   读事件通知文件描述符集合,一个1024个字节的数组,传入传出参数
 * @param writefds  写事件通知文件描述符集合
 * @param exceptfds 异常事件通知文件描述符集合
 * @param timeout   如果没有事件发生,select函数阻塞的时长,传null则是一直阻塞
 * @return
 */
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

// 初始化fd_set,全部置0
void FD_ZERO_DEFINE(fd_set *set);

// 对fd_set中指定fd置为1
void FD_SET_DEFINE(int fd, fd_set *set);

// 对fd_set中指定fd置为0
void FD_CLR_DEFINE(int fd, fd_set *set);

// 某个fd是否被置为1
int FD_ISSET_DEFINE(int fd, fd_set *set);


void cb(int sig) {

}

int main() {
    signal(SIGINT, cb);

    // 1. 创建socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    printf("server fd = %d \n", fd);

    // 2. 绑定端口
    struct sockaddr_in sock;
    memset(&sock, 0x00, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(8084);
    sock.sin_addr.s_addr = htons(INADDR_ANY);

    bind(fd, (const struct sockaddr *) &sock, sizeof(sock));

    // 3. 监听端口
    listen(fd, 128);

    // 4. 开始监听

    // 5. 注册select监听链接事件

    // 使用temp是因为,select轮寻的时候,会把已经注册的,置为1的,没有事件的fd的值置为0
    fd_set read_set, temp;
    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);

    // fd从0开始
    int max_fd = fd + 1;

    while (1) {
        temp = read_set;

        int ret = select(max_fd, &temp, NULL, NULL, NULL);

        if (ret == -1) {
            perror("select error");
            break;
        }

        // 链接事件
        if (FD_ISSET(fd, &temp)) {
            int client_fd = accept(fd, NULL, NULL);

            // 注册读事件
            FD_SET(client_fd, &read_set);

            // 改变max_fd
            max_fd = client_fd > fd ? client_fd + 1 : fd + 1;

            printf(" receive client connect , fd = %d \n", client_fd);

            // 客户端文件描述符可读事件
        } else {
            // fd一定是readSet的最小值,因为linux分配时文件描述符时是递增的
            for (int i = fd + 1; i < max_fd; ++i) {
                if (FD_ISSET(i, &temp)) {

                    printf(" client readable , fd = %d \n", i);

                    char buf[1024] = {0};

                    int ret = read(i, buf, 1024);

                    if (ret == 0) {
                        printf("client closed.. , fd = %d \n", i);
                        FD_CLR(i, &read_set);
                    }

                    char send_buf[1024] = {0};

                    sprintf(send_buf, "server -> %s", buf);

                    write(i, send_buf, 1024);

                }
            }
        }

    }

    close(fd);
}