#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "error.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE  "esp32 pong game client"


int main()
{
  EXIT_ON_ERROR(SDL_Init(SDL_INIT_VIDEO) < 0, "failed to initialize SDL2: %s\n", SDL_GetError());

  SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  EXIT_ON_ERROR(!window, "failed to crate the game window: %s\n", SDL_GetError());

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  EXIT_ON_ERROR(!renderer, "failed to crate the game renderer: %s\n", SDL_GetError());

  SDL_Event event;
  bool quit = false;
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
          }  
        }
      } 
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 18, 18, 18));
    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
