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

#include "lib.h"

#define MESSAGE_BUFF_MAX        1000
#define ESP32_LOCAL_NETWORK_IP  "192.168.1.100"
#define PORT                    80

#define UNUSED(x) ((void)(x))

static int sockfd;

void sigint_handler(int sig) {
    UNUSED(sig);
    close(sockfd);
}

#define TEST_MESSAGE "this is a test message";  

void client_send_test(TestResult *test_result)
{
    test_result->return_code = 0;
    test_result->test_name   = "sending data from game client to the server";

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) { perror("sigaction"); TEST_FAILED; }

    struct sockaddr_in server;
    char *message = TEST_MESSAGE;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("failed to create the socket: %d\n", errno);
        TEST_FAILED;
    }

    printf("created the socket: sockfd = %d\n", sockfd);

    server.sin_addr.s_addr = inet_addr(ESP32_LOCAL_NETWORK_IP);
    server.sin_family      = AF_INET;
    server.sin_port        = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("failed to connect to the server\n");
        TEST_FAILED;
    }

    printf("connected to the socket\n"); 
    printf("sending: %s\n", message);

    if (send(sockfd, message, strlen(message), 0) < 0) {
        printf("failed to send the message\n");
        TEST_FAILED;
    } 

    close(sockfd);
    TEST_PASSED;
}
