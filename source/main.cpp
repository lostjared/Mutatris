#include<iostream>
#include<string>
#include"SDL.h"
#include"SDL_ttf.h"
#include<cstdlib>
#include"quadtris.hpp"


int main(int argc, char **argv) {
    const int width = 1280, height = 720;
    const int TEX_WIDTH = 1280, TEX_HEIGHT = 720;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return EXIT_FAILURE;
    }

    if(TTF_Init() < 0) {
        std::cerr << "Error could not initalize SDL_ttf...\n";
        std::cerr.flush();
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Mutatris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_Texture *main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEX_WIDTH, TEX_HEIGHT);
    bool running = true;
    SDL_Event event;
    

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        SDL_SetRenderTarget(renderer, main_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, main_texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(main_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
