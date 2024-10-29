
#include<iostream>
#include<string>
#include<cstdlib>
#include<sstream>
#include"SDL.h"
#include"SDL_ttf.h"
#include"quadtris.hpp"
#include"argz.hpp"
#include"util.hpp"
#include"intro.hpp"
#include"game_object.hpp"
#include<memory>
#ifdef FOR_WASM
#include<emscripten/emscripten.h>
#endif
#include"version_info.hpp"
#ifdef HAS_SOUND
#include"sound.hpp"
#endif

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

    Argz<std::string> argz(argc, argv);
    argz.addOptionSingleValue('p', "path")
    .addOptionSingleValue('r', "Resolution")
    .addOptionSingle('f', "fullscreen mode")
    .addOptionDoubleValue('P', "path", "path to assets")
    .addOptionDoubleValue('R', "resolution", "window resolution")
    .addOptionDouble('F', "fullscreen", "Full screen mode");


    int value = 0;
    Argument<std::string> arg;
    bool full = false;
    std::cout << "Mutatris v" << version_info << ": written by Jared Bruni\n";

    try {
        while((value = argz.proc(arg)) != -1) {
            switch(value) {
                case 'f':
                case 'F':
                full = true;
                break;
                case 'p':
                case 'P':
                    util::path = arg.arg_value;
                    break;
                
                case 'R':
                case 'r': {
                    std::string resolution = arg.arg_value;
                    auto pos = resolution.find("x");
                    if(pos != std::string::npos) {
                        width = std::stoi(resolution.substr(0, pos));
                        height = std::stoi(resolution.substr(pos+1));
                        if(width == 0 || height == 0) {
                            std::cerr << "Mutatris: Invalid resolution.\n";
                        }
                        std::cout << "Mutatris: Setting resolution: " << width << "x" << height << "\n";
                    }
                }
                    break;
           }
        }
    } catch(const ArgException<std::string> &e) {
        std::cerr << "Mutatris: Syntax Error: " << e.text() << "\n";
    }

#ifdef HAS_SOUND
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
#else
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
#endif
        return EXIT_FAILURE;
    }

    if(TTF_Init() < 0) {
        std::cerr << "Mutatris: Error could not initalize SDL_ttf...\n";
        std::cerr.flush();
        return EXIT_FAILURE;
    }

    TTF_Font *fnt = TTF_OpenFont(util::getFilePath("font.ttf").c_str(), 18);
    if(!fnt) {
        std::cerr << "Mutatris: Error initalizing font..\n";
        std::cerr.flush();
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

#ifdef HAS_SOUND
    snd::init();
#endif


    SDL_Window* window = SDL_CreateWindow("Mutatris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, full == true ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
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

    SDL_Surface *icon = util::loadSurface("block_red.png");

    SDL_SetWindowIcon(window, icon);

    main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEX_WIDTH, TEX_HEIGHT);
    running = true;
    
    obj::setObject(new obj::IntroObject());
    obj::object->load(renderer);

#ifdef FOR_WASM
    emscripten_set_main_loop(eventProc, 0, 1);
#else    
    while (running) {
        eventProc(); 
    }
#endif
 
    obj::object.reset();
#ifdef HAS_SOUND
    snd::release();
#endif
    SDL_DestroyTexture(main_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(icon);
    TTF_CloseFont(fnt);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
