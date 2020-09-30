#include "fcntl.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/stat.h"
#include "string.h"
#include "stdlib.h"
#include "signal.h"

/**
 * kill函数是用来发送信号的,而不是发送kill信号
 *
 * @param __pid  发送给的进程pid
 * @param __sig  发送的信号
 */


// 1. pid大于0,pid是信号欲送往的进程的标识 (最常用)
// 2. pid等于0,信号将送往所有与调用kill()的那个进程属同一个使用组的进程
// 3. pid等于-1,信号将送往所有调用进程有权给其发送信号的进程,除了进程1(init)
// 4. pid小于-1,信号将送往以-pid为组标识的进程

int kill(__pid_t __pid, int __sig);

int main() {

    // 发送信号给signal.out程序
    char buf[64] = {0};

    read(STDIN_FILENO, buf, 64);

    int pid = atoi(buf);

    kill(pid, SIGINT);

    printf("发送信号成功~ \n");
}


