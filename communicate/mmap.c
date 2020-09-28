#include "unistd.h"
#include "sys/mman.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FILE_NAME "test_mmap.txt"

/**
 *(文件本身需要有内容,不可以是空文件)
 * @param addr      要映射的数据地址,一般传NULL,由os来分配地址
 * @param length    内存地址的大小
 * @param prot      保护模式 : PROT_READ,PROT_WRITE , PROT_READ | PROT_WRITE
 * @param flags     flags参数决定了当前进程修改的内容是否对其他进程可见,以及是否将内存中的数据写入到文件中 : MAP_SHARED , MAP_PRIVATE
 * @param fd        使用内存映射的文件
 * @param offset    文件偏移位,一般0
 * @return          映射后的内存地址
 */
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

/**
 * 取消对内存地址的映射
 * @return
 */
int munmap(void *start, size_t len);

void readMmap(void *addr);

void writeMmap(void *addr);

int main(int argc) {
    int fd = 0;

    if (access(FILE_NAME, F_OK) != 0) {
        fd = open(FILE_NAME, O_RDWR | O_CREAT);
    } else {
        fd = open(FILE_NAME, O_RDWR);
    }

    void *addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // 文件可以立即关闭,因为此时不再采用普通io,而是内存映射的方式读写文件
    close(fd);

    if (argc == 1) {
        printf("write start \n");
        writeMmap(addr);
        printf("write end \n");
    } else {
        printf("read start \n");
        readMmap(addr);
        printf("read end \n");
    }

    munmap(addr, 1024);
}

void readMmap(void *addr) {
    int i = 0;
    while (i++ < 10) {
        printf("%s \n", addr);

        sleep(1);
    }
}

void writeMmap(void *addr) {
    char buf[512] = {0};
    read(STDIN_FILENO, buf, 512);
    memcpy(addr, buf, strlen(buf) + 1);
    perror("error : ");
}