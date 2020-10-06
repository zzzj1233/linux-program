#include <zconf.h>
#include "signal.h"
#include "stdio.h"
/**
 * 1. 未决信号集,当进程接受到信号时,内核会填充该进程[未决信号集]对应的[信号标识]
 * 2. 如果阻塞信号集对应的标识没有值,则继续走下面的流程 -> SIG_ERR : return error | SIG_DFL : default action | SIG_IGN : ignore | customer function point
 * 3. 否则,进程将不会收到该信号,相当于信号被屏蔽了(阻塞)
 * 4. 阻塞信号集都是由人为填充的
 * 5. 所以说,以下api主要用于阻塞信号接收信号
 */

/**
 * __sigset_t,一个仅包含了一个 long int(64bit) 属性的[结构体]
 * 因为刚好只有64的信号, kill -l 查看所以信号
 */

// 1. 将结构体所有值置0
int sigemptyset(sigset_t *set);

// 2. 将结构体所有值置1
int sigfillset(sigset_t *set);

// 3. 将结构体指定信号位置1
int sigaddset(sigset_t *set, int sign);

// 4. 将结构体指定信号位置0
int sigdelset(sigset_t *set, int sign);

// 5. 判断某个信号在结构体中是否为1
// 如果为1返回1
int sigismember(const sigset_t *set, int sign);

/**
 * @param __how
 * 1. SIG_BLOCK
 *    block_set | __set
 * 2. SIG_UNBLOCK
 *    block_set & ^__set
 * 3. SIG_SETMASK
 *    block_set = __set
 * @param __set
 *    __sigset_t 结构体,要设置的内容
 * @param __oset
 *    __oset原先的阻塞信号集结构体
 * @return
 */
int sigprocmask(int __how, const sigset_t *__restrict __set, sigset_t *__restrict __oset);

/**
 * 获取当前进程的未决信号集
 * @param sigset_t  传出参数
 */
int sigpending(sigset_t *sigset_t);

int main() {
    // 1. 创建结构体
    sigset_t sigset;

    // 2. 清空
    sigemptyset(&sigset);

    // 3. 阻塞 ctrl + c 信号
    sigaddset(&sigset, SIGINT);
    // 阻塞 ctrl + z
    sigaddset(&sigset, SIGTSTP);

    // 4. 填充到阻塞信号集
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    // 5. 创建结构体用于获取未决信号集
    sigset_t pend;

    for (int i = 0; i < 50; ++i) {
        // 5.1 清空
        sigemptyset(&pend);

        // 5.2 获取
        sigpending(&pend);

        // 5.3 查看未决信号集前10个信号的状态
        for (int j = 1; j < 11; ++j) {
            if (sigismember(&pend, j)) {
                printf("1");
            } else {
                printf("0");
            }
        }
        printf("\n");
        sleep(1);
    }

}

