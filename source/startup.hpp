#ifndef _STARTINTRO_H__
#define _STARTINTRO_H__

#include"game_object.hpp"
#include"SDL.h"
#include"SDL_ttf.h"

namespace obj {

    class StartupObject : public GameObject {
    public:
        StartupObject() = default;
        virtual ~StartupObject();
        virtual void draw(SDL_Renderer *renderer) override;
        virtual void event(SDL_Renderer *renderer, SDL_Event &e) override;
        virtual void load(SDL_Renderer *renderer) override;
    protected:
        int cur_screen = 0;
        SDL_Texture  *logo = nullptr, *jb_logo = nullptr;
    };
}

#endif