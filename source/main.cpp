#include<iostream>
#include<string>
#include<cstdlib>
#include<sstream>
#include"SDL.h"
#include"SDL_ttf.h"
#include"quadtris.hpp"
#include"argz.hpp"

#ifdef FOR_WASM
std::string path = "/assets";
#else
std::string path = "assets";
#endif

std::string getFilePath(const std::string &filename) {
    std::ostringstream stream;
    stream << path << "/" << filename;
    return stream.str();
}

int main(int argc, char **argv) {
    int width = 1280, height = 720;
    
    const int TEX_WIDTH = 1280, TEX_HEIGHT = 720;

    Argz<std::string> argz(argc, argv);
    argz.addOptionSingleValue('p', "path")
    .addOptionSingleValue('r', "Resolution");

    int value = 0;
    Argument<std::string> arg;
    
    try {
        while((value = argz.proc(arg)) != -1) {
            switch(value) {
                case 'p':
                    path = arg.arg_value;
                    break;
                case 'r': {
                    std::string resolution = arg.arg_value;
                    auto pos = resolution.find("x");
                    if(pos != std::string::npos) {
                        width = std::stoi(resolution.substr(0, pos));
                        height = std::stoi(resolution.substr(pos+1));
                        if(width == 0 || height == 0) {
                            std::cerr << "Invalid resolution.\n";
                        }
                        std::cout << "Setting resolution: " << width << "x" << height << "\n";
                    }
                }
                    break;
            }
        }
    } catch(const ArgException<std::string> &e) {
        std::cerr << "Syntax Error: " << e.text() << "\n";
    }




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
