#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "unistd.h"
#include "sys/wait.h"

// 当子进程退出后,linux内核将会给他的父进程发送sigchld信号,我们可以监听该信号,完成对子进程的回收

void cb(int sig) {
    while (1) {
        // If PID is (pid_t) -1, match any process.
        int ret = waitpid(-1, NULL, WNOHANG);

        // 没有可回收的子进程
        if (ret == 0) {
            break;
        } else if (ret > 0) {
            printf("回收子进程 pid = [%d] \n", ret);
            continue;
        } else {
            break;
        }
    }
}

int main() {
    // 1. 先注册,防止子进程在注册前就关闭了,否则将无法收到回调
    signal(SIGCHLD, cb);

    // 2. 循环fork三个子进程
    int i = 0;
    for (; i < 3; ++i) {
        int pid = fork();

        // 子进程
        if (pid == 0) {
            break;
        }
    }

    // 第一个子进程
    if (i == 0) {
        sleep(1);
        printf("first child exit pid = %d ... \n", getpid());
        return 0;
    }

    if (i == 1) {
        sleep(2);
        printf("second child exit pid = %d  ... \n", getpid());
        return 0;
    }

    if (i == 2) {
        sleep(3);
        printf("third child exit pid = %d ... \n", getpid());
        return 0;
    }

    if (i == 3) {
        char buf[64] = {0};
        read(STDIN_FILENO, buf, 64);
        printf("parent exit... \n");
    }


}