#ifndef __GAME_OBJECT__H_
#define __GAME_OBJECT__H_

#include "SDL.h"

namespace obj {
    class GameObject {
    public:
        virtual ~GameObject() = default;
        virtual void draw(SDL_Renderer *renderer) = 0;
        virtual void event(SDL_Event &e) = 0;
        virtual void load(SDL_Renderer *renderer) = 0;
    };
}

#endif