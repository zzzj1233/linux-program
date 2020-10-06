#include "stdio.h"
#include "string.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "unistd.h"

int main() {
    // 1. 创建客户端socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 链接
    struct sockaddr_in server;
    memset(&server, 0x00, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(8086);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret = connect(fd, (const struct sockaddr *) &server, sizeof(server));

    if (ret == -1) {
        printf("链接被拒绝 \n");
        return -1;
    }

    while (1) {
        char buf[1024] = {0};

        fgets(buf, 1024, stdin);

        if (strcmp(buf, "exit") == 0) {
            break;
        }

        // 发送数据
        int write_ret = write(fd, buf, 1024);

        if (write_ret == -1) {
            perror("发送数据失败 \n");
            return -1;
        }


        // 读取数据

        char read_buf[1024] = {0};

        read(fd, read_buf, 1024);

        printf("接收到服务端数据 : %s \n", read_buf);
    }

    printf("客户端退出");

    close(fd);
}