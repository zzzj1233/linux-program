#include "signal.h"

/**
 * raise函数只能给当前进程(自己),发送指定信号
 * @param __sig     要发送的信号
 * @return
 */
int raise(int __sig);

int main() {
    return 0;
}

