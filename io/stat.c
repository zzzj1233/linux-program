#include <sys/stat.h>
#include "stdlib.h"
#include "stdio.h"
#include<pwd.h>

int stat(const char *pathname, struct stat *statbuf);


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("offer filepath");
        return -1;
    }

    char *fileName = argv[1];

    struct stat *s = malloc(sizeof(struct stat));

    stat(fileName, s);

    struct passwd *user;
    user = getpwuid(s->st_uid);

    printf("size : [%ld], uid : [%d] ,uname : [%s] ,uDir [%s] , gid : [%ld] \n", s->st_size, s->st_uid, user->pw_name, user->pw_dir, s->st_size);

    // file permission

    // file type

    free(s);
}