#ifndef __GAME_OVER__HPP__
#define __GAME_OVER__HPP__

#include"game_object.hpp"
#include"SDL.h"
#include"SDL_ttf.h"

namespace obj {
    class GameOverObject : public GameObject {
    public:
        GameOverObject(int scr, int clr);
        virtual ~GameOverObject();
        
        virtual void draw(SDL_Renderer *renderer) override;
        virtual void event(SDL_Renderer *renderer, SDL_Event &e) override;
        virtual void load(SDL_Renderer *renderer) override;
    protected:
        int score = 0, clears = 0;
        SDL_Texture *bg = nullptr;
        TTF_Font *font = nullptr;
    };
}




#endif