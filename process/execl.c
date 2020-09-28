#include "stdio.h"
#include "unistd.h"

// 1. 一般在fork出子进程后,不会根据pid判断让子进程执行自己的逻辑(太过麻烦)
//
// 2. 而是使用execl(path,name,...args,NULL)函数,让子进程执行他应该执行的程序
//
//        path : 执行的程序路径
//        name : 一般填写程序名 , ps -ef 显示的进程名称
//        args : 变参,路径参数
//        NULL : 告诉函数变参结束
//
// 3. 执行execl函数,如果失败,继续执行后续代码,如果成功,将不会返回执行结果,即不会执行后续代码

void forkProcess() {
    int pid = fork();
    if (pid == 0) {
        execl("./out/process/execl.c.out", "execl", "Fork", "Hello", "World", NULL);
        perror("execel error");
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        forkProcess();
    } else {

        for (int i = 1; i < argc; ++i) {
            printf("arg : [%d] = [%s] \n", i, argv[i]);
        }

    }
}