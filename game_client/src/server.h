#ifndef CONNECT_H
#define CONNECT_H

#define MSG_BUFFER_MAX 1024

// messages
#define M_GAME_START  "M_GAME_START"
#define M_GAME_END    "M_GAME_END"
#define M_DUMP_BOARD  "M_DUMP_BOARD"

#define M_OK          "M_OK"
#define M_FAIL        "M_FAIL"

char *server_send_and_read(const char *msg); 

#endif
