#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

/**
 *
 * @param __newthread          传出参数,获取线程号
 * @param __attr               传出参数,线程属性,无需关注
 * @param __start_routine      线程执行函数
 * @param __arg                线程执行函数参数
 * @return
 */

extern int pthread_create(pthread_t *__restrict __newthread,
                          const pthread_attr_t *__restrict __attr,
                          void *(*__start_routine)(void *),
                          void *__restrict __arg);

void *run(void *arg) {
    char *buf = malloc(64);
    memset(buf, 0x00, 64);

    sprintf(buf, "thread:%lu", pthread_self());

    // 使用api退出线程,对该线程使用了join的线程可以接收到该返回值
    pthread_exit(buf);

    // 对该线程使用了join的线程可以接收到该返回值
//    return buf;
}// -1 error

int main() {
    pthread_t t;

    int error = pthread_create(&t, NULL, run, NULL);

    if (error != 0) {
        strerror(error);
        return -1;
    }

    void *ret = NULL;

    // 等待线程执行完毕
    // 否则,主线程执行完毕,子线程仍在执行,进程不会,这一点和py一样,和java相反
    pthread_join(t, &ret);

    printf("ret = %s \n", ret);

    free(ret);
}
