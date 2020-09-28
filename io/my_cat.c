#include "stdio.h"
#include<unistd.h>
#include <fcntl.h>
#include "stdlib.h"

int main(int argv, char **args) {

    if (argv < 2) {
        printf("offer filename ! \n");
        return -1;
    }

    char *fileName = args[1];

    int fd = open(fileName, O_RDONLY);

    if (fd == -1) {
        printf("file not exists");
        return -1;
    }

    char *buff = malloc(124);

    int ret = -1;

    while ((ret = read(fd, buff, 124)) != 0) {
        write(1, buff, ret);
    }

    if (buff) {
        free(buff);
    }

    if (fd) {
        close(fd);
    }
}

