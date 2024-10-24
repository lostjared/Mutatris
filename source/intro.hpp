#ifndef _INTRO_H__
#define _INTRO_H__

#include"game_object.hpp"
#include"SDL.h"
#include"SDL_ttf.h"

namespace obj {

    class IntroObject : public GameObject {
    public:
        IntroObject() = default;
        virtual ~IntroObject();
        virtual void draw(SDL_Renderer *renderer) override;
        virtual void event(SDL_Renderer *renderer, SDL_Event &e) override;
        virtual void load(SDL_Renderer *renderer) override;
    protected:
        SDL_Texture *logo = nullptr;
        TTF_Font *font = nullptr;
        TTF_Font *large_font = nullptr;
    };
}

#endif