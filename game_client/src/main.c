#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "server.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 960
#define WINDOW_TITLE  "esp32 pong game client"

#define FONT_PATH     "VictorMono-Regular.ttf"
#define FONT_SIZE     60 

#define FPS           60
#define BASE_PLAYER_X 20

typedef struct {
  size_t player1_y;
  size_t player2_y;
  size_t player_branch_len;
} Game_Info;

void draw_text(SDL_Renderer *renderer, const char *text, int size, int x, int y, SDL_Color fg, SDL_Color bg)
{
  TTF_Font *font = TTF_OpenFont(FONT_PATH, size);
  EXIT_ON_ERROR(!font, "failed to load the game font: "FONT_PATH"\n");

  SDL_Surface *text_surface = TTF_RenderText_Shaded(font, text, fg, bg);

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_Rect msg_rect = {
    .x = x, 
    .y = y, 
    .w = WINDOW_WIDTH, 
    .h = 100
  };
  SDL_RenderCopy(renderer, texture, NULL, &msg_rect);

  SDL_FreeSurface(text_surface);
  SDL_DestroyTexture(texture);
  TTF_CloseFont(font);
}

void draw_players(SDL_Renderer *renderer, Game_Info info)
{
  printf("br len: %ld\n", info.player_branch_len);

  SDL_Rect rect1 = {BASE_PLAYER_X, WINDOW_HEIGHT/2 - info.player1_y - info.player_branch_len, 50, info.player_branch_len*100};
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderDrawRect(renderer, &rect1);
  
  SDL_SetRenderDrawColor(renderer, 0x18, 0x18, 0x18, 0xff);
}

Game_Info get_updated_game_info()
{
  Game_Info info;

  char buffer[MSG_BUFFER_MAX];
  buffer[MSG_BUFFER_MAX] = '\0';
  sprintf(buffer, M_DUMP_BOARD);

  char *response = server_send_and_read(buffer);

  int i = 0;
  for (char *c = strtok(response, ","); c != NULL; c = strtok(NULL, ",")) {
    switch (i) {
      case 0: info.player1_y = atoi(c);         break;
      case 1: info.player2_y = atoi(c);         break;
      case 2: info.player_branch_len = atoi(c); break;
    }
    i++;
  }

  return info;
}

int main(int argc, char *argv[])
{
  EXIT_ON_ERROR(argc < 3, "please provide player the names: ./esp32_pong_game_client player1 player2");

  EXIT_ON_ERROR(SDL_Init(SDL_INIT_VIDEO) < 0, "failed to initialize SDL2: %s\n", SDL_GetError());
  EXIT_ON_ERROR(TTF_Init() < 0, "failed to initialize SDL2_ttf: %s\n", TTF_GetError());

  SDL_Window *window     = SDL_CreateWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  EXIT_ON_ERROR(!window, "failed to crate the game window: %s\n", SDL_GetError());

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  EXIT_ON_ERROR(!renderer, "failed to crate the game renderer: %s\n", SDL_GetError());

  SDL_Event event;
  bool quit           = false;
  bool game_started   = false;
  bool end_game_after = false;
  bool start_updating = false;
  Game_Info game_info;

  /*
   * Game control keys:
   * Escape/Q   - close the window
   * Space      - start the game
   */

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: { quit = true; break; }
        case SDL_KEYDOWN: {
          switch (event.key.keysym.sym) {
            case SDLK_q:      { quit = true; break; }
            case SDLK_ESCAPE: { quit = true; break; }
            case SDLK_SPACE:  { if (!game_started) game_started = true; }
          }  
        }
      }
    }
   
    // start the game
    if (game_started) { 
      char buffer[MSG_BUFFER_MAX];
      buffer[MSG_BUFFER_MAX] = '\0';
      sprintf(buffer, M_GAME_START"|%s|%s", argv[1], argv[2]);

      char *response = server_send_and_read(buffer);
      if (strncmp(response, M_OK, strlen(M_OK))) printf(M_GAME_START": reponse and sent message are incompatible!\n");

      game_started = false;
      end_game_after = true;
      start_updating = true;
    }

    // set game background color to #181818
    SDL_SetRenderDrawColor(renderer, 0x18, 0x18, 0x18, 0xff);
    SDL_RenderClear(renderer);

    if (start_updating) game_info = get_updated_game_info(), draw_players(renderer, game_info);

    if (!game_started) {
      // draw the game start text
      SDL_Color fg = {0xff, 0xff, 0xff};
      SDL_Color bg = {0x00, 0x00, 0x00};

      (void)fg;
      (void)bg;
      draw_text(renderer, "press <space> to start the game", FONT_SIZE, 0, WINDOW_HEIGHT/2 - FONT_SIZE, fg, bg);
    }
  

    SDL_RenderPresent(renderer);
  }

  if (end_game_after) {
    // end the game
    char buffer[MSG_BUFFER_MAX];
    buffer[MSG_BUFFER_MAX] = '\0';
    sprintf(buffer, "%s", M_GAME_END);

    char *response = server_send_and_read(buffer);
    if (strncmp(response, M_OK, strlen(M_OK))) printf(M_GAME_END": reponse and sent message are incompatible!\n");
  }

  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
