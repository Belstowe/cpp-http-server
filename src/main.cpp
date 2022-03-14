#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Uninet.h"

#define BUFLEN 256

int buff_work(socket_t sockClient) {
    char buf[BUFLEN + 1];
    ssize_t msgLength;

    memset(buf, 0, BUFLEN);
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
    socket_t sockMain, sockClient;
    socklen_t length;
    sockaddr_in servAddr;
    fd_set rfds;
    fd_set afds;
    socket_t fd, nfds;

    #ifdef _WIN32
    struct WSAData w_data;

    if (WSAStartup(MAKEWORD(2, 2), &w_data) != 0) {
        perror("TCP_Server: Couldn't startup Windows Sockets 2.\n");
        exit(1);
    }
    #endif

    sockMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockMain == WIN(INVALID_SOCKET) NIX(-1)) {
        perror("TCP_Server: Couldn't open TCP socket.\n");
        exit(1);
    }
    memset(&servAddr, 0, sizeof(servAddr));

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