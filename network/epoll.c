#include "sys/epoll.h"
#include "arpa/inet.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "fcntl.h"

/**
 * @param size  epoll树大小,这个值必须大于0,epoll内部会动态扩充
 * @return      epoll文件描述符, < 0 就是error
 */
int epoll_create(int size);

/**
 *
 * @param epfd      epoll文件描述符
 * @param op        操作,EPOLL_CTL_ADD,EPOLL_CTL_MOD,EPOLL_CTL_DEL
 * @param fd        要操作的文件描述符
 * @param event     传入参数,要挂在到epoll树上的结构体 ( epoll树挂载的都是一个个struct epoll_event的指针 )
 * @return
 */
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

struct epoll_event_define {
    uint32_t events;
    /**
     * Epoll events 可取值 : EPOLLIN,EPOLLERR,,EPOLLOUT
     *
     * 控制触发模式
     *
     * 1. 不设置,默认模式 : 水平触发模式
     *    只要缓冲区里面有可读数据,epoll_wait函数就会返回一次,假设客户端发送了500KB的数据,但是每次read只读5kb,则循环内的epoll_wait函数返回100次
     *
     * 2. | EPOLLET : 边缘触发模式
     *    客户端发送一次,epoll_wait函数就会返回一次,但是不知道客户端到底一次性发送了多大的数据在缓冲区所以需要把fd设置为非阻塞,保证每次读都能把所有的数据读出来
     *
     */
    epoll_data_t data;      /* User data variable */
};

typedef union epoll_data_define {
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t_define;

/**
 * 和select函数一样的作用,让内核通知io事件
 * @param epfd              epoll文件描述符
 * @param events            传出参数,epoll_event结构体数组,epoll会把发送io事件的epoll_event结构体填充进数组
 * @param maxevents         epoll_event结构体数组最大长度
 * @param timeout           等待时长, -1 = 阻塞等待 , 0 = 不阻塞 , > 0 = 阻塞时长
 * @return                  -1 = 出错, 发生的io事件个数
 */
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);


void cb(int sig) {}

int main() {

    signal(SIGINT, cb);

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // 1. 创建epoll树
    int epfd = epoll_create(30);

    // 2. 把serverSocket描述符添加到epoll树中
    struct epoll_event ev;
    memset(&ev, 0x00, sizeof(ev));
    // 监听可读事件
    ev.events = EPOLLIN;
    // data属性是一个联合体,业务比较简单,挂载fd即可,后续可根据fd判断是serverSocket还是clientSocket
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

    struct sockaddr_in server;
    memset(&server, 0x00, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8086);
    server.sin_addr.s_addr = INADDR_ANY;
    bind(fd, (const struct sockaddr *) &server, sizeof(server));

    listen(fd, 32);

    // 3. 等待epoll唤醒
    struct epoll_event *events = malloc(sizeof(struct epoll_event) * 20);

    printf("wait ... \n");

    memset(events, 0x00, sizeof(struct epoll_event) * 20);

    while (1) {
        // 永久阻塞
        int ret = epoll_wait(epfd, events, 20, -1);
        if (ret < 0) {
            perror("epoll_wait error ");
            break;
        }

        // ret 就是发生的io事件的个数
        for (int i = 0; i < ret; ++i) {
            struct epoll_event event = events[i];

            if (!(event.events & EPOLLIN)) {
                continue;
            }


            // server
            if (event.data.fd == fd) {
                int client = accept(fd, NULL, NULL);

                // 注册client
                struct epoll_event cli_event;
                memset(&cli_event, 0x00, sizeof(cli_event));

                // 边缘检测模式
                cli_event.events = EPOLLIN | EPOLLET;
                cli_event.data.fd = client;

                // 非阻塞
//                int flag = fcntl(client, F_GETFL);
//                flag |= O_NONBLOCK;
//                fcntl(client, F_SETFL, flag);

                epoll_ctl(epfd, EPOLL_CTL_ADD, client, &cli_event);

                printf("接受到客户端链接 : client fd = %d \n", client);
            } else {
                // client
                char buf[1024] = {0};

                int ret = read(event.data.fd, buf, 1024);

                if (ret == 0) {
                    printf("client closed...\n");
                    // 从epoll中移除监听
                    epoll_ctl(epfd, EPOLL_CTL_DEL, event.data.fd, NULL);

                    close(event.data.fd);
                    continue;
                }

                printf("接收到客户端消息 : client fd = %d \n", event.data.fd);

                char send_buf[1024] = {0};
                sprintf(send_buf, "epoll server : %s", buf);

                write(event.data.fd, send_buf, 1024);
            }
        }

    }

    close(fd);
    free(events);
    close(epfd);
}

