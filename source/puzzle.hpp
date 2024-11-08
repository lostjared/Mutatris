#ifndef _PUZZLE_H__
#define _PUZZLE_H__

#include"game_object.hpp"
#include"SDL.h"
#include"SDL_ttf.h"
#include"quadtris.hpp"
#include<vector>

namespace obj {

    class PuzzleObject : public GameObject {
    public:
        PuzzleObject(int diff);
        virtual ~PuzzleObject();
        virtual void draw(SDL_Renderer *renderer) override;
        virtual void event(SDL_Renderer *renderer, SDL_Event &e) override;
        virtual void load(SDL_Renderer *renderer) override;
        void drawGrid(SDL_Renderer *renderer, SDL_Texture *tex, int focus);
    protected:
        puzzle::PuzzleGame game;
        std::vector<SDL_Texture *> blocks;
        std::vector<SDL_Texture *> backgrounds;
        SDL_Texture *pause_menu = nullptr;
        bool paused = false;
        int cur_level = 0;
        int cur_focus = 0;
        SDL_Texture *game_textures[4] = {nullptr};
        TTF_Font *font = nullptr;
        TTF_Font *paused_small, *paused_large;
        void twistColors();
    };

}

#endif