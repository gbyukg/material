#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define SV_SOCK_PATH "/tmp/ud_ucase"
#define BUF_SIZE 10

void errExit(char *msg)
{
    extern int errno;
    perror(msg);
    exit(errno);
}

int main(void)
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
        errExit("socket wrong");

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
        errExit("remove wrong");

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (const struct sockaddr*)&svaddr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind wrong");

    for (;;) {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *)&claddr, &len);

        if (numBytes == -1)
            errExit("recvfrom wrong");

        printf("Server received %ld bytes from %s\n", (long)numBytes, claddr.sun_path);

        for (j = 0; j < numBytes; j++)
            buf[j] = toupper((unsigned char) buf[j]);

        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *)&claddr, len) != numBytes)
            errExit("send");
    }
    exit(EXIT_SUCCESS);
}
