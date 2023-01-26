#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "unused.h"

// 1. messages incoming from the client
// 2. messages sent back from the client
#define FOREACH_MSG(m)  \
    m(M_GAME_START)     \
    m(M_GAME_END)       \
    m(M_OK)             \
    m(M_FAIL)           \
    m(M_UNKNOWN_ACTION) \

#define GENERATE_ENUM(e) e,
#define GENERATE_STRING(s) #s,

typedef enum {
    FOREACH_MSG(GENERATE_ENUM)
} Msg;

static char *MSG_STRING[] = {
   FOREACH_MSG(GENERATE_STRING) 
};

void process_message(char *rx_buffer, char *out_string)
{
    if (!strncmp(rx_buffer, MSG_STRING[M_GAME_START], strlen(MSG_STRING[M_GAME_START]))) {
	    strcpy(out_string, MSG_STRING[M_OK]);
    } else if (!strncmp(rx_buffer, MSG_STRING[M_GAME_END], strlen(MSG_STRING[M_GAME_END]))) {
	    strcpy(out_string, MSG_STRING[M_OK]);
    } else {
        strcpy(out_string, MSG_STRING[M_UNKNOWN_ACTION]);
    }
}
