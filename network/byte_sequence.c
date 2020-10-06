#include "stdio.h"
#include "arpa/inet.h"

// 32位int 网络转主机字节序
uint32_t ntohl(uint32_t __netlong);

// 16位int 网络转主机字节序
uint16_t ntohs(uint16_t __netshort);

// 32位int 主机转网络字节序
uint32_t htonl(uint32_t __hostlong);

// 16位int 主机转网络字节序
uint16_t htons(uint16_t __hostshort);

int main() {

    int a = 0x1234;

    char *p = (char *) &a;

    // 34
    printf("%x \n", *(p + 0));
    // 12
    printf("%x \n", *(p + 1));

    // 低位地址放低位值
    // 高位地址放高位值

    // 但是网络传输(也称主机字节序)时,要求
    // 低位地址放高位值
    // 高位地址放低位值

    // 34120000
    printf("htonl -> %x \n", htonl(a));

    // 3412
    printf("htons -> %x \n", htons(a));
}

