#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "time.h"
#include "string.h"
#include<unistd.h>

int main(int argv, char *args[]) {

    if (argv < 2) {
        printf("offer filename ! \n");
        return -1;
    }

    time_t t;
    time(&t);

    char *fileName = args[1];

    int fd = open(fileName, O_WRONLY | O_CREAT, 0666);

    char *time = ctime(&t);

    write(fd, time, strlen(time) + 1);

    printf("make successful ~ \n");

    return 0;
}
