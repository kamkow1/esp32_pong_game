#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "server.h"

#define SERVER_ADDR     "192.168.1.100"
#define SERVER_PORT     80
#define RECV_BUFFER_MAX 2048

static char RECV_BUFFER[RECV_BUFFER_MAX];
static int  SOCKET_FD;

char *server_send_and_read(const char *msg) 
{
  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));

  EXIT_ON_ERROR((SOCKET_FD = socket(AF_INET, SOCK_STREAM, 0)) < 0, "failed to create the socket\n");

  server.sin_family      = AF_INET;
  server.sin_port        = htons(SERVER_PORT);
  server.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  EXIT_ON_ERROR(connect(SOCKET_FD, (struct sockaddr*)&server, sizeof(server)) < 0, "failed to connect\n");

  int sent_bytes = send(SOCKET_FD, msg, strlen(msg), 0);
  EXIT_ON_ERROR(sent_bytes < 0, "failed to send the message: %s\n", msg);

  memset(RECV_BUFFER, 0, sizeof(RECV_BUFFER));
  int recv_bytes = read(SOCKET_FD, RECV_BUFFER, sizeof(RECV_BUFFER));

  EXIT_ON_ERROR(recv_bytes < 0, "failed to receive\n");

  close(SOCKET_FD);
  return RECV_BUFFER;
}

