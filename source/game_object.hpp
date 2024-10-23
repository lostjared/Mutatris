#ifndef __GAME_OBJECT__H_
#define __GAME_OBJECT__H_

#include "SDL.h"
#include<memory>

namespace obj {
    class GameObject {
    public:
        virtual ~GameObject() = default;
        virtual void draw(SDL_Renderer *renderer) = 0;
        virtual void event(SDL_Event &e) = 0;
        virtual void load(SDL_Renderer *renderer) = 0;
    };

    inline std::unique_ptr<obj::GameObject> object;
    inline void setObject(obj::GameObject *o) {
        object.reset(o);
    }
}

#endif