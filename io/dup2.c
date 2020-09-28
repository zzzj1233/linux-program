#include <unistd.h>
#include "stdio.h"
#include "string.h"
#include "fcntl.h"

// int dup2(int oldfd, int newfd);

/**
 * this different from <code> int dup(int oldFd) </code> is
 *
 * if newFd is NULL,it same as <code> int dup(int oldFd) </code>
 *
 * else , os will close newFd, and copy from oldFd
 */

int main() {

    int oldFd = open("udp2_test.txt", O_RDWR | O_CREAT);

    char content[] = "write by oldFd";

    write(oldFd, content, sizeof(content) - 1);

    // copy fd
    int newFd = oldFd + 1;

    dup2(oldFd, newFd);

    // set point
    lseek(newFd, 0, SEEK_SET);

    // read
    char buf[64];
    memset(buf, 0x00, 64);

    read(newFd, buf, 64);

    printf("read by new Fd ----> %s", buf);

    close(oldFd);
    close(newFd);

    return 0;
}


