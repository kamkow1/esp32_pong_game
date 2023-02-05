#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "unused.h"
#include "game.h"

// 1. messages incoming from the client
// 2. messages sent back from the client
#define FOREACH_MSG(m)  \
    m(M_GAME_START)     \
    m(M_GAME_END)       \
    m(M_PLAYER_MOVE)    \
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

void parse_message_format(char *rx_buffer, char **split_args)
{
  split_args[MAX_ARGS] = ARGS_END_FLAG;
  char *str = strtok(rx_buffer, ARG_SPLIT_DELIM);
  size_t i = 0;
  while (str) {
    split_args[i] = str;
    str = strtok(NULL, ARG_SPLIT_DELIM);
    i++;
  }
}

void process_message(char *rx_buffer, char *out_string)
{
  // handle M_GAME_START
  if (!strncmp(rx_buffer, MSG_STRING[M_GAME_START], strlen(MSG_STRING[M_GAME_START]))) {
    char *split_args[MAX_ARGS];
    parse_message_format(rx_buffer, split_args);

    char *player1_name = split_args[1], *player2_name = split_args[2];
    char *game_handler_status = game_start(player1_name, player2_name) ? MSG_STRING[M_OK] : MSG_STRING[M_FAIL];
    strcpy(out_string, game_handler_status);

    reset_board(); 
    update_player(PLAYER1);
    update_player(PLAYER2);
    debug_print_board();

  // handle M_PLAYER_MOVE
  } else if (!strncmp(rx_buffer, MSG_STRING[M_PLAYER_MOVE], strlen(MSG_STRING[M_PLAYER_MOVE]))) { 
    char *split_args[MAX_ARGS];
    parse_message_format(rx_buffer, split_args);

    int player_id = atoi(split_args[1]);
    int y         = atoi(split_args[2]);
    Player *pl    = player_id == 1 ? PLAYER1 : PLAYER2;
    move_player(pl, y);
    strcpy(out_string, MSG_STRING[M_OK]);

    reset_board(); 
    update_player(pl);
    debug_print_board();

	// handle M_GAME_END
  } else if (!strncmp(rx_buffer, MSG_STRING[M_GAME_END], strlen(MSG_STRING[M_GAME_END]))) {
    game_end();
    strcpy(out_string, MSG_STRING[M_OK]);
  } else strcpy(out_string, MSG_STRING[M_UNKNOWN_ACTION]); // failed to find a correspoding game action
}
