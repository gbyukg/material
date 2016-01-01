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
    struct addrinfo hints, *result = NULL, *rp = NULL;
    struct sockaddr_in *addr;
    char ip[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo("www.baidu.com", "80", &hints, &result) != 0)
        errExit("getaddrinfo wrong");

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        addr = (struct sockaddr_in *)rp->ai_addr;
        printf("IP: %s\n", inet_ntop(AF_INET, (struct in_addr *)&addr->sin_addr, ip, INET_ADDRSTRLEN));
    }

    freeaddrinfo(result);
    return 0;
}
