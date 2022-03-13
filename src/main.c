#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <errno.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 256

int buff_work(int sockClient) {
    char buf[BUFLEN + 1];
    ssize_t msgLength;

    bzero(buf, BUFLEN);
    if ((msgLength = recv(sockClient, buf, BUFLEN, 0)) < 0) {
        perror("TCP_Server: Couldn't receive a message.\n");
        return -1;
    }
    else if (msgLength == 0)
        return 1;

    printf("TCP_Server data:\n");
    printf("\t- Client socket: %d\n", sockClient);
    printf("\t- Message length: %zd\n", msgLength);
    printf("\t- Message: %s\n\n", buf);

    return 0;
}

int main() {
    int sockMain, sockClient;
    socklen_t length;
    struct sockaddr_in servAddr;
    fd_set rfds;
    fd_set afds;
    int fd, nfds;

    if ((sockMain = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("TCP_Server: Couldn't open TCP socket.\n");
        exit(1);
    }
    bzero( (char *) &servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = 0;

    if (bind(sockMain, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
        perror("TCP_Server: Couldn't bind socket.\n");
        exit(1);
    }

    length = sizeof(servAddr);
    if (getsockname(sockMain, (struct sockaddr *)&servAddr, &length)) {
        perror("TCP_Server: Couldn't call 'getsockname'.\n");
        exit(1);
    }

    if (listen(sockMain, 5) < 0) {
        perror("TCP_Server: Couldn't establish connections.");
        exit(1);
    }

    nfds = getdtablesize();
    FD_ZERO(&afds);
    FD_SET(sockMain, &afds);

    printf("TCP_Server: Port %d.\n", ntohs(servAddr.sin_port));
    while (1) {
        memcpy(&rfds, &afds, sizeof(rfds));
        if (select(nfds, &rfds, NULL, NULL, NULL) < 0) {
            perror("TCP_Server: Couldn't select a socket.\n");
        }
        if (FD_ISSET(sockMain, &rfds)) {
            if ((sockClient = accept(sockMain, NULL, NULL)) < 0) {
                perror("TCP_Server: Faulty client socket.\n");
                exit(1);
            }
            FD_SET(sockClient, &afds);
        }
        
        for (fd = 0; fd < nfds; fd++) {
            if (fd != sockMain && FD_ISSET(fd, &rfds)) {
                if (buff_work(fd)) {
                    close(fd);
                    FD_CLR(fd, &afds);
                }
            }
        }
    }
}