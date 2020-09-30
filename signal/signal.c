#include "signal.h"
#include "unistd.h"
#include "stdio.h"

#define FILE_NAME "pid.fifo"

// __handler宏定义,如果不使用自定义回调函数可以使用这些
#define    SIG_ERR     ((__sighandler_t) -1)    /* Error return.  */
#define    SIG_DFL     ((__sighandler_t)  0)    /* Default action.  */
#define    SIG_IGN     ((__sighandler_t)  1)    /* Ignore signal.  */

/**
 *
 * @param __sig         信号标识,使用SIGINT宏定义进去查看头文件,包含了unix下的所有信号,或者使用kill -l命令,参考 https://www.cnblogs.com/wuyepeng/p/9790396.html 查看命令
 * @param __handler     回调函数,可选linux提供的常量
 * @return
 */
__sighandler_t signal(int __sig, __sighandler_t __handler);

typedef void (*__sighandler_t)(int);

void handler(int sign) {
    printf("接受到信号 : %d \n", sign);
}

int main() {
    printf("start,waiting signal... \n");

    // 用于和kill.out通信 ---
    printf("pid = %d \n", getpid());

    // SIGINT
    // ctrl + z产生
    signal(SIGINT, handler);

    char buf[64] = {0};
    read(STDIN_FILENO, buf, 64);

    printf("exit ... \n");
}