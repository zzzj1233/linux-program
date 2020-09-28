#include <unistd.h>
#include "stdio.h"
#include "string.h"

int dup(int oldfd);

int main() {
    // copy fd from oldfd

    int fd = STDOUT_FILENO;

    printf("STDOUT_FILENO FD = %d \n", fd);

    int copyFd = dup(fd);

    printf("COPY FD = %d \n", copyFd);

    char str[] = "Hi,I'm Stdout \n";

    write(fd, str, sizeof(str) - 1);

    char str2[] = "Hi,I'm Copy Fd  \n";

    write(copyFd, str2, sizeof(str2) - 1);

    // if is normal fd

    // close(fd)
    // close(copyFD)
}