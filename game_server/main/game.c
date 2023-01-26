#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include "game.h"

typedef struct {
  size_t y;
  int points;
} Player;

bool game_start(const char *player1_id, const char *player2_id)
{
  printf("player1 = %s, player2 = %s\n", player1_id, player2_id);
  return true;
}
