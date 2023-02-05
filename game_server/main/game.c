#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"

#include "game.h"

Player *PLAYER1;
Player *PLAYER2;
bool   *BOARD;

void debug_print_board()
{
  for (size_t x = 0; x < BOARD_WIDTH+2; x++) printf("~");
  printf("\n");

  for (size_t x = 0; x < BOARD_WIDTH; x++) {
    printf("%c", 'a' + x);
    for (size_t y = 0; y < BOARD_HEIGHT; y++) 
      if (BOARD[BOARD_WIDTH*x+y]) printf("@"); else printf(" ");
    printf("%c\n", 'a' + x);
  }

  for (size_t x = 0; x < BOARD_WIDTH+2; x++) printf("~");
  printf("\n");
}

void init_player(Player *pl, char *player_name, size_t id)
{
  pl->name    = player_name;
  pl->points  = 0;
  pl->y       = BOARD_HEIGHT/2;
  pl->id      = id;
}

void init_draw_players()
{
  for (size_t x = 0; x < BOARD_WIDTH; ++x)
    for (size_t y = 0; y < BOARD_HEIGHT; ++y)
      for (size_t player_y_size = 0; player_y_size < PLAYER_Y_SIZE; ++player_y_size) BOARD[BOARD_WIDTH*x+y] = true;
}

bool game_start(char *player1_id, char *player2_id)
{
  if (!(PLAYER1 = (Player*) malloc(sizeof(Player)))) return false;
  if (!(PLAYER2 = (Player*) malloc(sizeof(Player)))) return false;
  if (!(BOARD   = (bool*)   malloc(sizeof(bool)*BOARD_WIDTH*BOARD_HEIGHT))) return false;
  
  init_player(PLAYER1, player1_id, 1);
  init_player(PLAYER2, player2_id, 2);
  init_draw_players();
  
  return true;
}

bool game_end()
{ 
  free(PLAYER1);
  free(PLAYER2);
  free(BOARD);

  return true;
}

void move_player(Player *player, size_t y) { player->y += y; }

void set_xy(size_t x, size_t y, bool s) { BOARD[BOARD_WIDTH*x+y] = s; }

void reset_board()
{
  for (size_t x = 0; x < BOARD_WIDTH; ++x)
    for (size_t y = 0; y < BOARD_HEIGHT; ++y) set_xy(x, y, false);
}

void update_player(Player *pl)
{
  int pad_bottom = pl->y-PLAYER_Y_SIZE;
  int pad_top    = pl->y+PLAYER_Y_SIZE;

  if (pad_bottom < 0)           { printf("pad is lower than the board height\n");  return; }
  if (pad_top > BOARD_HEIGHT-1) { printf("pad is higher than the board height\n"); return; }

  for (size_t x = pad_bottom; x < pad_top; x++) set_xy(x, pl->id == 1 ? 0 : BOARD_WIDTH - 1, true);
}

