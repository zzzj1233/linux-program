#include "stdio.h"
#include "string.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "unistd.h"

/**
 * @param af        协议族,常用的有AF_INET和AF_INET6
 * @param type      数据传输方式,一般使用SOCK_STREAM
 * @param protocol  传输协议,常用IPPROTO_TCP和IPPROTO_UDP
 * @return          fd
 */
int socket(int af, int type, int protocol);

/**
 *
 * @param __fd      socket文件描述符
 * @param __addr    sockaddr结构体指针,一般使用sockaddr_in,更易操作
 * @param __len     结构体大小
 * @return
 */
extern int bind(int fd, __CONST_SOCKADDR_ARG addr, socklen_t socket_struct_len);

struct sockaddr_in_define {
    sa_family_t sin_family;   // 使用的ip协议族,一般使用AF_NET和AF_INET6
    uint16_t sin_port;     // 16个字节的端口号,要绑定的端口,需要使用网络字节序
    struct in_addr sin_addr;     // 32个字节IP地址,in_addr的属性s_addr 是一个整数,而IP地址是一个字符串,所以需要 inet_addr() 函数进行转换
    char sin_zero[8];  // 不使用，一般用0填充
};

/**
 *
 * @param backlog 请求队列的最大长度
 *
    当套接字正在处理客户端请求时，如果有新的请求进来，套接字是没法处理的，只能把它放进缓冲区，待当前请求处理完毕后，再从缓冲区中读取出来处理
    如果不断有新的请求进来,它们就按照先后顺序在缓冲区中排队，直到缓冲区满。这个缓冲区，就称为请求队列（Request Queue）。

    缓冲区的长度（能存放多少个客户端请求）可以通过 listen() 函数的 backlog 参数指定，但究竟为多少并没有什么标准，可以根据你的需求来定，并发量小的话可以是10或者20。

    如果将 backlog 的值设置为, fd = %d SOMAXCONN，就由系统来决定请求队列长度，这个值一般比较大，可能是几百，或者更多。

    当请求队列满时，就不再接收新的请求，对于 Linux，客户端会收到 ECONNREFUSED 错误，对于 Windows，客户端会收到 WSAECONNREFUSED 错误
 */
extern int listen(int fd, int backlog);

/**
 * @param arrd      传出参数,表示客户端的sockaddr_in的结构体指针
 */
extern int accept(int fd, __SOCKADDR_ARG arrd, socklen_t *__restrict addr_len);


int main() {
    // 1. 创建socket
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in sock;
    memset(&sock, 0x00, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(8081);
    sock.sin_addr.s_addr = htonl(INADDR_ANY);

    // 2. 绑定端口
    bind(fd, (const struct sockaddr *) &sock, sizeof(sock));

    // 3. 监听端口
    listen(fd, 128);

    struct sockaddr_in client;
    memset(&client, 0x00, sizeof(client));

    socklen_t client_len = sizeof(client);

    int client_fd = accept(fd, (struct sockaddr *) &client, &client_len);

    if (client_fd < 0) {
        perror("Server Accept Failed ");
        return -1;
    }

    printf("客户端端口 -> %d \n", client.sin_port);

    printf("客户端ip -> %s \n", inet_ntoa(client.sin_addr));

    // 4. 接收请求
    while (1) {
        char buf[1024] = {0};

        // 读取客户端发送的消息
        int ret = read(client_fd, buf, sizeof(buf));

        if (ret == -1) {
            perror("read error ");
            return -1;
        } else if (ret == 0){
            printf("client closed... \n");
            close(client_fd);
            break;
        }

        printf("receive from client : %s \n", buf);

        // 发送给客户端
        char send_buff[1024] = {0};
        strcat(send_buff, "server : ");
        strcat(send_buff, buf);

        write(client_fd, send_buff, sizeof(send_buff));
    }

    close(client_fd);
    close(fd);
    printf("server closed ... \n");
}