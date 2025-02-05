#include"SDL.h"
#include"SDL_image.h"

namespace png {
    SDL_Surface* LoadPNG(const char* file) {
        return IMG_Load(file);      
    }
}