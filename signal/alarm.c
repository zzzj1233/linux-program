#include <zconf.h>
#include "signal.h"
#include "stdio.h"

/**
 * 延时信号回调函数,会在指定的秒数后给当前进程发送SIGALRM信号
 * SIGALRM是在定时器终止时发送给进程的信号
 *
 * 重复调用会覆盖之前的
 *
 * @return  剩余执行的秒数
 */
unsigned int alarm(unsigned int __seconds);

void cb(int sig) {
    printf("接收到信号 : %d \n", sig);
}

int main() {

    // 1. 注册回调函数
    signal(SIGALRM, cb);

    alarm(2);

    sleep(20);

    // 程序被回调后,将会从挂起状态切换到就绪状态
    // 所以不会等待20s再退出
    // 而是被回调后就退出了
    printf("end ...\n");

    return 0;
}


