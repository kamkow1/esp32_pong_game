#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#define BOARD_WIDTH   20 
#define BOARD_HEIGHT  20
#define PLAYER_HEIGHT 10

// player y size is PLAYER_Y_SIZE*2 
// means + - 1 pixel from the middle of the pad 
#define PLAYER_Y_SIZE 2 
#define GAME_TAG      "game"

typedef struct {
  // the player is a thin, vertical rectangle
  // so we use the Y axis to move the player
  size_t id;
  size_t y;
  int    points;
  char   *name;
} Player;

extern Player *PLAYER1;
extern Player *PLAYER2;
extern bool   *BOARD;

bool game_start(char *player1_id, char *player2_id);
bool game_end();
void move_player(Player *player, size_t y);
void debug_print_board();
void reset_board();
void update_player(Player *pl);
void draw_board_to_string(char *string_buffer);

#endif
