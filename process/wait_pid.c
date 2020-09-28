#include "sys/wait.h"
#include "unistd.h"
#include "stdio.h"

// pid_t waitpid(pid_t pid, int *stat_loc, int options);
//
// 1.  waitpid和wait的区别在于
// 1.1 waitpid可以指定等待结束的进程,如果传-1和wait一样,等待一个最先结束的进程
// 1.2 waitpid可以通过options设置为非阻塞

// 2.  如果设置为非阻塞(阻塞:0,非阻塞WNOHANG(1))
//
// 2.1 子进程结束,返回子进程PID
// 2.2 子进程未结束,返回-1

int main() {
    // 所以说,waitpid最大的好处在于,可以指定等待结束的子进程
    // 也可以非阻塞执行
}