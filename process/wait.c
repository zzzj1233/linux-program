#include "stdio.h"
#include "unistd.h"
#include <sys/wait.h>
#include "stdlib.h"

int main(int argc,char ** argv) {

    int status = -1;

    if (argc >= 2) {
        int pid = fork();

        if (pid == 0) {
            int sleepTime = atoi(argv[1]);

            printf("我是子线程,我要休眠 [%d] s,pid = [%d] \n", sleepTime,getpid());

            // 子线程休眠
            sleep(sleepTime);

            return 0;
        }
    }
    int pid = wait(&status);

    if (pid == -1) {
        printf("kill nothing \n");
    } else {
        printf("killed child process, pid = %d  \n", pid);

        if (WIFEXITED(status)) {
            printf("子进程正常退出 \n");

            // 使子进程休眠足够长的时间,然后使用kill杀死演示
        } else if (WIFSIGNALED(status)) {
            printf(" 子进程收到信号退出 \n");
        }

    }

}