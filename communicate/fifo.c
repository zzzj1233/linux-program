// 1. fifo是一个逻辑文件,大小为0字节

// 2. 本质是一块内核缓冲区,被unix抽象为一个文件,这样就可以像操作文件一样进行通信

// 3. 我们可以使用mkfifo命令或者mkfifo函数创建一个fifo文件

#include "sys/stat.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"

#define FILE_NAME "testFifo"

void writeFifo() {
    char readBuf[1024];

    int fifoFd = open(FILE_NAME, O_RDWR);

    while (1) {

        memset(readBuf, 0x00, 1024);

        int size = read(STDIN_FILENO, readBuf, 1024);

        if (strlen(readBuf) == 1) {
            close(fifoFd);
            return;
        }

        // 写到fifo文件中
        write(fifoFd, readBuf, size);
    }

}

void readFifo() {
    // 从fifo文件中读数据
    int fifoFd = open(FILE_NAME, O_RDWR);

    char readBuf[1024];
    while (1) {
        read(fifoFd, readBuf, 1024);

        if (strlen(readBuf) == 1) {
            break;
        }

        printf("read process read -> %s \n", readBuf);
        memset(readBuf, 0x00, 1024);
    }
    close(fifoFd);
    printf("write channel close , read process exit ... \n");
}

int main(int argc) {

    // fifo文件不存在则创建
    if (access(FILE_NAME, F_OK) != 0) {
        mkfifo(FILE_NAME, 0666);
    }

    // 不传参数开启写进程
    if (argc == 1) {
        printf("write process start \n");
        writeFifo();
    } else {
        printf("read process start .. \n");
        readFifo();
    }

    return 0;
}






