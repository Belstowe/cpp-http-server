#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <errno.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 256
#define TID_NUM 65536

struct {
    pthread_mutex_t st_mutex[TID_NUM];
} GLOBAL;

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

    printf("TCP_Server data (thread %zu):\n", pthread_self());
    printf("\t- Client socket: %d\n", sockClient);
    printf("\t- Message length: %zd\n", msgLength);
    printf("\t- Message: %s\n\n", buf);

    return 0;
}

int handler(int sockClient) {
    pthread_mutex_lock(&GLOBAL.st_mutex[sockClient]);
    while (!buff_work(sockClient));
    close(sockClient);
    pthread_mutex_unlock(&GLOBAL.st_mutex[sockClient]);
    pthread_exit(NULL);
}

int main() {
    int sockMain, sockClient;
    socklen_t length;
    struct sockaddr_in servAddr;
    pthread_t th;
    pthread_attr_t ta;

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

    pthread_attr_init(&ta);
    pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
    for (unsigned i = 0; i < TID_NUM; i++)
        pthread_mutex_init(&GLOBAL.st_mutex[i], 0);

    printf("TCP_Server: Port %d.\n", ntohs(servAddr.sin_port));
    while (1) {
        if ((sockClient = accept(sockMain, NULL, NULL)) < 0) {
            perror("TCP_Server: Faulty client socket.\n");
            exit(1);
        }

        if (pthread_create(&th, &ta, (void *(*)(void *))handler, (void *)sockClient) < 0) {
            perror("TCP_Server: Couldn't thread.");
            exit(1);
        }
    }
}