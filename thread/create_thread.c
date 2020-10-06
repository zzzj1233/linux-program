#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

/**
 *
 * @param __newthread          ��������,��ȡ�̺߳�
 * @param __attr               ��������,�߳�����,�����ע
 * @param __start_routine      �߳�ִ�к���
 * @param __arg                �߳�ִ�к�������
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

    // ʹ��api�˳��߳�,�Ը��߳�ʹ����join���߳̿��Խ��յ��÷���ֵ
    pthread_exit(buf);

    // �Ը��߳�ʹ����join���߳̿��Խ��յ��÷���ֵ
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

    // �ȴ��߳�ִ�����
    // ����,���߳�ִ�����,���߳�����ִ��,���̲���,��һ���pyһ��,��java�෴
    pthread_join(t, &ret);

    printf("ret = %s \n", ret);

    free(ret);
}
