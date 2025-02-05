
#include<iostream>
#include<string>
#include<cstdlib>
#include<sstream>
#include"SDL.h"
#include"SDL_ttf.h"
#include"quadtris.hpp"
#include"util.hpp"
#include"intro.hpp"
#include"game_object.hpp"
#include"version_info.hpp"
#include"sound.hpp"
#include"startup.hpp"
#include"SDL_image.h"

SDL_Texture *main_texture;
bool running = true;
SDL_Event event;
SDL_Renderer *renderer;

void eventProc() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            running = false;
        }
        obj::object->event(renderer, event);
    }
    SDL_SetRenderTarget(renderer, main_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    obj::object->draw(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, main_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

}

int main(int argc, char **argv) {
    int width = 1280, height = 720;
    const int TEX_WIDTH = 1280, TEX_HEIGHT = 720;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) < 0) {
        return EXIT_FAILURE;
    }

    IMG_Init(IMG_INIT_PNG);

    if(TTF_Init() < 0) {
        SDL_Log("Mutatris: Error could not initalize SDL_ttf...\n");
        return EXIT_FAILURE;
    }

    TTF_Font *fnt = TTF_OpenFont(util::getFilePath("font.ttf").c_str(), 18);
    if(!fnt) {
        SDL_Log("Mutatris: Error initalizing font..\n");
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    snd::init();

    SDL_Window* window = SDL_CreateWindow("Mutatris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_RenderSetLogicalSize(renderer, 1280, 720);

    SDL_Surface *icon = util::loadSurface("block_red.png");
    SDL_SetWindowIcon(window, icon);
    main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEX_WIDTH, TEX_HEIGHT);
    running = true;
    util::initJoystick();
    obj::setObject(new obj::StartupObject());
    obj::object->load(renderer);

    while (running) {
        eventProc(); 
    }
 
    obj::object.reset();
    snd::release();
    util::closeJoystick();
    SDL_DestroyTexture(main_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(icon);
    TTF_CloseFont(fnt);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
