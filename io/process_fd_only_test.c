#include "stdio.h"
#include "stdlib.h"
#include<unistd.h>
#include <fcntl.h>

// test the fd is only in one process

int main(int argc, char **argv) {

    if (argc > 1) {
        close(STDERR_FILENO);
        printf("closed STDERR_FILENO \n");
    }

    int fd = open("./main.c", O_RDONLY);

    printf("process pid %d start... \n", getpid());

    printf("fd = %d \n", fd);

    char buf[64];

    read(STDIN_FILENO, buf, 64);

    printf("process pid %d end...  \n", getpid());
}