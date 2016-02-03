#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DB_MAX_CLOSE 8192

int becomeDeamon(int);

int main(void)
{
    return 0;
}

int becomeDeamon(int flags)
{
    int maxfd, fd;

    switch (fork()) {
    case -1: return -1;
    case 0: break; // 子进程继续执行
    default: _exit(EXIT_SUCCESS); // 父进程直接退出
    }

    // 在子进程中执行 setsid() 函数
    // 来创建一个新的会话, 使之与当前的控制终端断开连接
    if (setsid() == -1)
        return -1;

    switch (fork()) {
        case -1: return -1;
        case 0: break; // 子子进程继续执行, 子子进程必定不是新回话的首进程, 防止后续操作中打开新的控制终端.
        default: _exit(EXIT_SUCCESS); // 子进程退出执行, 子进程是新会话的会话首进程
    }

    // 清除进程的 umask
    umask(0);

    // 修改进程根目录
    chdir("/");

    // 获取系统允许的最大可以打开的文件描述符
    maxfd = sysconf(_SC_OPEN_MAX);
    if (maxfd == -1)
        maxfd = DB_MAX_CLOSE;

    // 关闭所有父进程中打开的文件描述符
    for (fd = 0; fd < maxfd; fd++)
        close(fd);

    // 打开 /dev/null, 文件描述符应当为 0
    fd = open("/dev/null", O_RDWR);
    // 如果不等于标准输入文件描述符, 报错
    if (fd != STDIN_FILENO)
        return -1;

    // 重定向标准输出到标准输入
    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
        return -1;

    // 重定向标准错误输出到标准输入
    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
        return -1;

    return 0;
}
