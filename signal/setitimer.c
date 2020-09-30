#include "sys/time.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "signal.h"

/**
 * 位于/sys/time.h头文件
 * @param which
    ITIMER_REAL : 以系统真实的时间来计算(常用值)
    ITIMER_VIRTUAL : 以该进程在用户态下花费的时间来计算
    ITIMER_PROF : 以该进程在用户态下和内核态下所费的时间来计算
 * @param value
 *  时间
 * @param ovalue
 *  一般传NULL
 * @return
 */
int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue);

/**
struct timeval {
    // 秒
    long tv_sec;
    // 微秒
    long tv_usec;
};

struct itimerval {
    // 间隔调用时间
    struct timeval it_interval;
    // 初始延时调用时间
    struct timeval it_value;
};
 */

void cb(int sig) {
    printf("接收到信号, sig = [%d] \n", sig);
}

int main() {
    struct itimerval timeVal;
    memset(&timeVal, 0x00, sizeof(struct itimerval));

    // 每隔1s调用一次
    timeVal.it_interval.tv_sec = 1;
    // 3s后开始第一次调用,以后每隔1s执行一次
    timeVal.it_value.tv_sec = 3;

    // 注册信号回调函数
    signal(SIGALRM, cb);

    setitimer(ITIMER_REAL, &timeVal, NULL);

    char buf[64] = {0};
    read(STDIN_FILENO, buf, 64);

    printf("end ...\n");

    return 0;
}
