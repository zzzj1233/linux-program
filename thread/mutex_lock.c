#include "pthread.h"
#include "stdio.h"

#define COUNT 100000

int num = 0;

// 1. 创建互斥锁
pthread_mutex_t mutexLock;

void *cb(void *arg) {
    // 上锁,C语言中的互斥锁不是可重入锁
    pthread_mutex_lock(&mutexLock);

    // 非阻塞尝试获取锁
    // pthread_mutex_trylock()

    for (int i = 0; i < COUNT; ++i) {
        num++;
    }
    // 解锁
    pthread_mutex_unlock(&mutexLock);
}

int main() {

    pthread_t thread1;
    pthread_t thread2;


    // 2. 初始化互斥锁
    pthread_mutex_init(&mutexLock, NULL);

    pthread_create(&thread1, NULL, cb, NULL);
    pthread_create(&thread2, NULL, cb, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_detach()

    // 3. 销毁锁
    pthread_mutex_destroy(&mutexLock);

    printf("num = %d \n", num);
}