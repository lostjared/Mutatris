#include"util.hpp"
#include"loadpng.hpp"

namespace util {

    std::vector<SDL_GameController *> stick;

    #ifdef FOR_WASM
    std::string path = "/assets";
    #else
    std::string path = "assets";
    #endif

    std::string getFilePath(const std::string &filename) {
        SDL_Log("Loading.... %s", filename.c_str());
       return filename;
    }

    void printText(SDL_Renderer *renderer,TTF_Font *font,int x, int y, const std::string &text, SDL_Color col) {
        SDL_Surface *surf = TTF_RenderText_Blended(font,text.c_str(), col);
        if(!surf) {
            SDL_Log("Mutatris: Error rendering text...\n");
            return;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        if(!tex) {
            SDL_Log("Mutatris: Error creating texture..\n");
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
            SDL_Log("Mutatris: Error could not open file: ");
            return NULL;
        }
        if(color)
            SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, key.r, key.g, key.b));

        w = surface->w;
        h = surface->h;

        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
        if(!tex) {
            SDL_Log("Mutatris: Error creating texture from surface..\n");
            return NULL;
        }
        SDL_FreeSurface(surface);
        return tex;
    }

    TTF_Font *loadFont(const std::string &filename, int size) {
        TTF_Font *fnt = TTF_OpenFont(getFilePath(filename).c_str(), size);
        if(!fnt) {
            SDL_Log("Mutatris: Error Opening Font: ");
            return NULL;
        }
        return fnt;
    }

    SDL_Surface *loadSurface(const std::string &name) {
        return png::LoadPNG(getFilePath(name).c_str());
    }

    void initJoystick() {

        for(int i = 0; i < SDL_NumJoysticks(); ++i) {
            SDL_GameController *stick_ = SDL_GameControllerOpen(i);
            if(!stick_) {
                std::cout << "Mutatris: Game Controller disabled..\n";
            } else {
                std::cout << "Mutatris: Game Controller: " << SDL_GameControllerName(stick_) << " enabled...\n";
            }
            stick.push_back(stick_);
        }
        SDL_GameControllerEventState(SDL_ENABLE);
    }

    void closeJoystick() {
        for(size_t i = 0; i < stick.size(); ++i) {
            SDL_GameControllerClose(stick[i]);
        }
    }

    void connectJoystick(SDL_Event &e) {
       if (e.type == SDL_CONTROLLERDEVICEADDED) {
            std::cout << "Controller attached for Player " << e.cdevice.which << "\n";
            SDL_GameController *gamec = SDL_GameControllerOpen(e.cdevice.which);
            if(gamec) {
                std::cout << "Connected Controller " << SDL_GameControllerName(gamec) << "\n";
                stick.push_back(gamec);
            } else {
                std::cout << "Failed to open controller: " << SDL_GetError() << ".\n";
            }
        }
        if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
            std::cout << "Controllers disconnected.\n";
            closeJoystick();
        }
    }

}