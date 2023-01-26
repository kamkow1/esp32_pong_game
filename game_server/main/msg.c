#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "unused.h"
#include "game.h"

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

#define MAX_ARGS        20
#define ARG_SPLIT_DELIM "|"
#define ARGS_END_FLAG   "#ARGS_END"

typedef enum {
    FOREACH_MSG(GENERATE_ENUM)
} Msg;

static char *MSG_STRING[] = {
   FOREACH_MSG(GENERATE_STRING) 
};

void process_message(char *rx_buffer, char *out_string)
{
  // handle M_GAME_START
  if (!strncmp(rx_buffer, MSG_STRING[M_GAME_START], strlen(MSG_STRING[M_GAME_START]))) {

	char *str = strtok(rx_buffer, ARG_SPLIT_DELIM);
	char *split_args[MAX_ARGS];
	split_args[MAX_ARGS] = ARGS_END_FLAG;

	size_t i = 0;
	while (str) {
	  split_args[i] = str;
	  str = strtok(NULL, ARG_SPLIT_DELIM);
	  i++;
	}

	char *player1_name = split_args[1];
	char *player2_name = split_args[2];
	char *game_handler_status = game_start(player1_name, player2_name) ? MSG_STRING[M_OK] : MSG_STRING[M_FAIL];
    strcpy(out_string, game_handler_status);
  } else if (!strncmp(rx_buffer, MSG_STRING[M_GAME_END], strlen(MSG_STRING[M_GAME_END]))) {
    strcpy(out_string, MSG_STRING[M_OK]);
  } else {
    strcpy(out_string, MSG_STRING[M_UNKNOWN_ACTION]);
  }
}
