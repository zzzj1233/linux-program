#include<sys/types.h>
#include<dirent.h>
#include "stdio.h"

// d_ino 此目录进入点的inode
// d_off 目录文件开头至此目录进入点的位移
// d_reclen name的长度，不包含NULL字符
// d_type d_name 所指的文件类型
// d_name 文件名

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("please offer dir path");
        return -1;
    }

    DIR *d = opendir(argv[1]);

    struct dirent *dt = NULL;

    while ((dt = readdir(d)) != NULL) {
        printf("%s \n", dt->d_name);
    }

    printf("\n");


    return 0;
}