#ifndef _PUZZLE_H__
#define _PUZZLE_H__

#include"game_object.hpp"
#include"SDL.h"
#include"SDL_ttf.h"
#include"quadtris.hpp"

namespace obj {

    class PuzzleObject : public GameObject {
    public:
        PuzzleObject() = default;
        virtual ~PuzzleObject();
        virtual void draw(SDL_Renderer *renderer) override;
        virtual void event(SDL_Renderer *renderer, SDL_Event &e) override;
        virtual void load(SDL_Renderer *renderer) override;
    protected:
        puzzle::PuzzleGame game;
    };

}

#endif