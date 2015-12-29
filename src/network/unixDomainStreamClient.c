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

int main(void)
{
    int cfd = 0;
    int numRead = 0;
    char buf[BUF_SIZE];
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(struct sockaddr_un));

    if ((cfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        errExit("socket wrong");

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(cfd, (const struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1)
        errExit("connect wrong");

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
        if (write(cfd, buf, numRead) != numRead)
            errExit("write wrong");

    if (numRead == -1)
        errExit("read wrong");

    return EXIT_SUCCESS;
}
