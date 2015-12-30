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

#define BUF_SIZE 10
#define PORT_NUM 50002

void errExit(char *msg)
{
    extern int errno;
    perror(msg);
    exit(errno);
}

int
main(int argc, char *argv[])
{
    struct sockaddr_in6 svaddr;
    int sfd, j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("%s host-address msg...\n", argv[0]);
        exit(0);
    }

    /* Create a datagram socket; send to an address in the IPv6 domain */

    sfd = socket(AF_INET6, SOCK_DGRAM, 0);      /* Create client socket */
    if (sfd == -1)
        errExit("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_port = htons(PORT_NUM);
    // 绑定到 IPv6 回环地址上
    svaddr.sin6_addr = in6addr_loopback;
    /*if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0) {*/
        /*printf("inet_pton failed for address '%s'", argv[1]);*/
        /*exit(1);*/
    /*}*/

    /* Send messages to server; echo responses on stdout */

    for (j = 2; j < argc; j++) {
        msgLen = strlen(argv[j]);
        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr,
                    sizeof(struct sockaddr_in6)) != msgLen)
            errExit("sendto");

        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        if (numBytes == -1)
            errExit("recvfrom");

        printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
    }

    exit(EXIT_SUCCESS);
}
