#ifndef _INTRO_H__
#define _INTRO_H__

#include"game_object.hpp"
#include"SDL.h"
#include"SDL_ttf.h"
#include<unordered_map>

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
        SDL_Texture *start_screen = nullptr;
        TTF_Font *font = nullptr, *medium_font;
        TTF_Font *large_font = nullptr;
        TTF_Font *small_font = nullptr;
        TTF_Font *diff_font = nullptr;
        int cur_screen = 0;
        int cursor_pos = 0;

    };
}

#endif