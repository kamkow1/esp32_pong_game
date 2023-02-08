#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "error.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE  "esp32 pong game client"

#define FONT_PATH     "VictorMono-Regular.ttf"
#define FONT_SIZE     32 

void draw_text(SDL_Surface *screen, const char *text, int size, int x, int y, SDL_Color fg, SDL_Color bg)
{
  TTF_Font *font = TTF_OpenFont(FONT_PATH, size);
  EXIT_ON_ERROR(!font, "failed to load the game font: "FONT_PATH"\n");

  SDL_Surface *text_surface = TTF_RenderText_Shaded(font, text, fg, bg);
  SDL_Rect text_loc = {x, y, 0, 0};
  SDL_BlitSurface(text_surface, NULL, screen, &text_loc);
  SDL_FreeSurface(text_surface);
  TTF_CloseFont(font);
}

int main()
{
  EXIT_ON_ERROR(SDL_Init(SDL_INIT_VIDEO) < 0, "failed to initialize SDL2: %s\n", SDL_GetError());

  SDL_Window *window     = SDL_CreateWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  EXIT_ON_ERROR(!window, "failed to crate the game window: %s\n", SDL_GetError());

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  EXIT_ON_ERROR(!renderer, "failed to crate the game renderer: %s\n", SDL_GetError());

  EXIT_ON_ERROR(TTF_Init() < 0, "failed to initialize SDL2_ttf: %s\n", TTF_GetError());


  SDL_Event event;
  bool quit         = false;
  bool game_started = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          quit = true;
          break;
        }

        case SDL_KEYDOWN: {
          switch (event.key.keysym.sym) {
            case SDLK_q: {
              quit = true;
              break;               
            }
            case SDLK_SPACE: {
              game_started = true; 
            }
          }  
        }
      } 
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 18, 18, 18));

    if (!game_started) {
      // draw the game start text
      SDL_Color fg = {0xff, 0xff, 0xff};
      SDL_Color bg = {0x00, 0x00, 0x00};
      draw_text(surface, "press <space> to start the game", FONT_SIZE, WINDOW_WIDTH/2 - FONT_SIZE - 240, WINDOW_HEIGHT/2 - FONT_SIZE, fg, bg);
    }

    SDL_UpdateWindowSurface(window);
  }

  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
