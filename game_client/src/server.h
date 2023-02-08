#ifndef CONNECT_H
#define CONNECT_H

#define MSG_BUFFER_MAX 1024

// messages
#define M_GAME_START  "M_GAME_START"
#define M_GAME_END    "M_GAME_END"

#define M_OK          "M_OK"
#define M_FAIL        "M_FAIL"

void server_connect();
void server_send(const char *msg);
char *server_read();
void server_disconnect();

#endif
