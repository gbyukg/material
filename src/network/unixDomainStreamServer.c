#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define BUF_SIZE 500
#define SOCKET_PATH "/tmp/socket"

int errExit(char *msg)
{
    extern int errno;
    perror(msg);
    exit(EXIT_FAILURE);
}

/*
 * UNIX domain 数据报服务器端
 */
int main(void)
{
    int sfd = 0, cfd = 0;
    int numRead = 0;
    char buf[BUF_SIZE];
    struct sockaddr_un addr, c_addr;
    socklen_t peer_addr_size = sizeof(struct sockaddr_un);

    /*
     * 初始化结构体
     * 确保结构体中的所有字节都被初始化
     */
    memset(&addr, 0, sizeof(struct sockaddr_un));

    // UNIX domain socket 只能为 AF_UNIX
    addr.sun_family = AF_UNIX;
    // 设定 socket 路径
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // 创建 socket
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        errExit("Socket wrong");

    // 若 socket 文件存在, 则需要先删除
    if (remove(SOCKET_PATH) == -1 && errno != ENOENT)
        errExit("remove wrong");

    // 绑定 socket
    if (bind(sfd,
                (const struct sockaddr *)&addr,
                sizeof(struct sockaddr_un)) == -1)
        errExit("Bind wrong");

    // 将 socket 设为被动模式(服务器)
    if (listen(sfd, SOMAXCONN) == -1)
        errExit("listen wrong");

    while (1) {
        // 接收请求, 若没有请求到达, 则使之处于阻塞状态
        if ((cfd = accept(sfd, (struct sockaddr *)&c_addr, &peer_addr_size)) == -1)
                errExit("Accept wrong");

        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
                errExit("write wrong");

        if (numRead == -1)
            errExit("read wrong");

        if (close(cfd) == -1)
            errExit("close wrong");
    }

    return EXIT_SUCCESS;
}
