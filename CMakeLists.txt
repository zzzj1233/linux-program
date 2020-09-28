cmake_minimum_required(VERSION 3.16)
project(test1 C)

set(CMAKE_C_STANDARD 99)

add_executable(test1 io/main.c io/time.c io/my_cat.c io/stat.c io/process_fd_only_test.c io/dup.c io/dup2.c io/dir.c process/fork.c process/fork_global_var.c
        process/execl.c
        process/zombie_process.c

        process/wait.c process/wait_pid.c communicate/pipe.c communicate/pipe_ps_ef.c communicate/non_blocking_pipe.c communicate/fifo.c communicate/mmap.c)