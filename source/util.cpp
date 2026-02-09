#include"util.hpp"
#include"loadpng.hpp"

namespace util {

    std::vector<SDL_GameController *> stick;
    std::vector<SDL_Joystick *> joysticks;

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

    void printText(SDL_Renderer *renderer,TTF_Font *font,int x, int y, const std::string &text, SDL_Color col) {
        SDL_Surface *surf = TTF_RenderText_Blended(font,text.c_str(), col);
        if(!surf) {
            std::cerr << "Mutatris: Error rendering text...\n";
            return;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        if(!tex) {
            std::cerr << "Mutatris: Error creating texture..\n";
            SDL_FreeSurface(surf);
            return;
        }
        SDL_Rect rc = {x,y,surf->w,surf->h};
        SDL_RenderCopy(renderer, tex, nullptr, &rc);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filename) {
        int w = 0, h = 0;
        return loadTexture(renderer, filename, w, h, false, {0,0,0,0});
    }
    
    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filename, int &w, int &h, bool color, SDL_Color key) {
        SDL_Surface *surface = png::LoadPNG(getFilePath(filename).c_str());
        if(!surface) {
            std::cerr << "Mutatris: Error could not open file: " << getFilePath(filename) << "\n";
            std::cerr.flush();
            exit(EXIT_FAILURE);
        }
        if(color)
            SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, key.r, key.g, key.b));

        w = surface->w;
        h = surface->h;

        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
        if(!tex) {
            std::cerr << "Mutatris: Error creating texture from surface..\n";
            std::cerr.flush();
            exit(EXIT_FAILURE);
        }
        SDL_FreeSurface(surface);
        return tex;
    }

    TTF_Font *loadFont(const std::string &filename, int size) {
        TTF_Font *fnt = TTF_OpenFont(getFilePath(filename).c_str(), size);
        if(!fnt) {
            std::cerr << "Mutatris: Error Opening Font: " << filename << "\n";
            std::cerr.flush();
            exit(EXIT_FAILURE);
        }
        return fnt;
    }

    SDL_Surface *loadSurface(const std::string &name) {
        return png::LoadPNG(getFilePath(name).c_str());
    }

    void initJoystick() {

        for(int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                SDL_GameController *stick_ = SDL_GameControllerOpen(i);
                if(!stick_) {
                    std::cout << "Mutatris: Game Controller disabled..\n";
                } else {
                    std::cout << "Mutatris: Game Controller: " << SDL_GameControllerName(stick_) << " enabled...\n";
                }
                stick.push_back(stick_);
            } else {
                SDL_Joystick *joy = SDL_JoystickOpen(i);
                if (!joy) {
                    std::cout << "Mutatris: Joystick disabled..\n";
                } else {
                    std::cout << "Mutatris: Joystick: " << SDL_JoystickName(joy) << " enabled...\n";
                }
                joysticks.push_back(joy);
            }
        }
        SDL_GameControllerEventState(SDL_ENABLE);
        SDL_JoystickEventState(SDL_ENABLE);
    }

    void closeJoystick() {
        for(size_t i = 0; i < stick.size(); ++i) {
            SDL_GameControllerClose(stick[i]);
        }
        stick.clear();
        for(size_t i = 0; i < joysticks.size(); ++i) {
            SDL_JoystickClose(joysticks[i]);
        }
        joysticks.clear();
    }

    void connectJoystick(SDL_Event &e) {
       if (e.type == SDL_CONTROLLERDEVICEADDED) {
            int index = e.cdevice.which;
            std::cout << "Controller attached at index " << index << "\n";
            SDL_GameController *gamec = SDL_GameControllerOpen(index);
            if(gamec) {
                std::cout << "Connected Controller: " << SDL_GameControllerName(gamec) << "\n";
                stick.push_back(gamec);
            } else {
                std::cout << "Failed to open controller: " << SDL_GetError() << ".\n";
            }
        }
        if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
            SDL_JoystickID id = e.cdevice.which;
            std::cout << "Controller disconnected (instance " << id << ").\n";
            for (auto it = stick.begin(); it != stick.end(); ++it) {
                SDL_Joystick *joy = SDL_GameControllerGetJoystick(*it);
                if (joy && SDL_JoystickInstanceID(joy) == id) {
                    SDL_GameControllerClose(*it);
                    stick.erase(it);
                    break;
                }
            }
        }
        if (e.type == SDL_JOYDEVICEADDED) {
            int index = e.jdevice.which;
            if (!SDL_IsGameController(index)) {
                SDL_Joystick *joy = SDL_JoystickOpen(index);
                if (joy) {
                    std::cout << "Connected Joystick: " << SDL_JoystickName(joy) << "\n";
                    joysticks.push_back(joy);
                } else {
                    std::cout << "Failed to open joystick: " << SDL_GetError() << "\n";
                }
            }
        }
        if (e.type == SDL_JOYDEVICEREMOVED) {
            SDL_JoystickID id = e.jdevice.which;
            std::cout << "Joystick disconnected (instance " << id << ").\n";
            for (auto it = joysticks.begin(); it != joysticks.end(); ++it) {
                if (SDL_JoystickInstanceID(*it) == id) {
                    SDL_JoystickClose(*it);
                    joysticks.erase(it);
                    break;
                }
            }
        }
    }

}