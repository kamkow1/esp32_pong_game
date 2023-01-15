#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>

#define MESSAGE_BUFF_MAX        1000
#define ESP32_LOCAL_NETWORK_IP  "192.168.1.100"
#define PORT                    80

#define UNUSED(x) ((void)(x))

static int sockfd;

void sigint_handler(int sig) {
    UNUSED(sig);
    close(sockfd);
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) perror("sigaction"), exit(1);

    struct sockaddr_in server;
    char message[MESSAGE_BUFF_MAX];
    message[MESSAGE_BUFF_MAX] = '\0';

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("failed to create the socket: %d\n", errno);
        exit(1);
    }

    printf("created the socket: sockfd = %d\n", sockfd);

    server.sin_addr.s_addr = inet_addr(ESP32_LOCAL_NETWORK_IP);
    server.sin_family      = AF_INET;
    server.sin_port        = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("failed to connect to the server\n");
        exit(1);
    }

    printf("connected to the socket\n");

    printf("send message: ");
    scanf("%s", message);
        
    printf("sending: %s\n", message);

    if (send(sockfd, message, strlen(message), 0) < 0) {
        printf("failed to send the message\n");
        exit(1);
    } 

    /*
    if (recv(sockfd, server_reply, MESSAGE_BUFF_MAX * 2, 0) < 0) {
        printf("failed to receive data from the server\n");
        exit(1);
    }

    printf("the server replied: %s\n", server_reply);
    */

    close(sockfd);

    return 0;
}
