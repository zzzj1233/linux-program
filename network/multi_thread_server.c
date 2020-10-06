#include "arpa/inet.h"
#include "string.h"
#include "stdio.h"
#include "unistd.h"
#include "pthread.h"
#include "signal.h"

int fd = 0;

void cb(int sig) {
    close(fd);
}


void *callback(void *arg) {
    int *client = arg;

    while (1) {
        char buf[1024] = {0};
        int ret = read(*client, buf, 1024);

        if (ret == -1) {
            perror("read error");
            break;
        }

        if (ret == 0) {
            printf("client closed ... \n");
            break;
        }

        char ret_buf[1024] = {0};

        sprintf(ret_buf, "thread_id : %lu , receive : %s", pthread_self(), buf);

        printf("%s \n", ret_buf);

        write(*client, ret_buf, sizeof(ret_buf));

    }

    close(*client);
}

int main() {
    // 1. 创建socket
    fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 注册信号事件
    signal(SIGINT, cb);

    // 3. 绑定端口
    struct sockaddr_in server;
    memset(&server, 0x00, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8083);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(fd, (const struct sockaddr *) &server, sizeof(server));

    // 4. 监听
    listen(fd, 128);

    int clients[] = {0};
    int i = 0;
    while (1) {
        // 5. 接收请求
        clients[i++] = accept(fd, NULL, NULL);
        if (clients[i - 1] == -1) {
            perror("accept error ");
            return 0;
        }


        // 6. 分发线程
        pthread_t pd;
        memset(&pd, 0x00, sizeof(pd));

        int ret = pthread_create(&pd, NULL, callback, &clients[i - 1]);

        if (ret == -1) {
            perror("创建线程失败 ");
            return -1;
        }

        // 线程自己释放资源
        pthread_detach(pd);
    }

}