#include "stdio.h"
#include "unistd.h"

// 1. 父进程Fork出子进程后,unix会拷贝一块完全相同的内存空间给子进程用
//
// 2. 对于仅仅是读取变量,对两者来说是相同的
//
// 3. 一旦父进程或者子进程修改了变量,unix将会使其指向另一块物理内存空间,然后修改值
//
// 4. 读时共享,写时复制

int abc = 123;

int main() {
    int pid = fork();

    // child
    if (pid == 0) {
        sleep(2);
        // child -> abc = 123
        printf("child -> abc = %d \n", abc);
    } else {
        abc = 321;
        // parent -> abc = 321
        printf("parent -> abc = %d \n", abc);
    }
}