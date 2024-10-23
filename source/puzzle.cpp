#include "puzzle.hpp"
#include<iostream>
#include<string>
#include"util.hpp"

namespace obj {

    PuzzleObject::~PuzzleObject() {
        std::cout << "Mutatris: Game Released...\n";
        for(int i = 0; i < blocks.size(); ++i) {
            SDL_DestroyTexture(blocks[i]);
        }
    }

    void PuzzleObject::draw(SDL_Renderer *renderer) {

    }

    void PuzzleObject::event(SDL_Renderer *renderer, SDL_Event &e)  {

    }

    void PuzzleObject::load(SDL_Renderer *renderer) {
        std::cout << "Mutatris: Game Init...\n";
        static const char *filenames[] = { "block_black.png", "block_clear.png", "block_dblue.png", "block_green.png", "block_ltblue.png", "block_orange.png", "block_red.png", "block_yellow.png", 0};
        for(int i = 0; filenames[i] != 0; ++i) {
            blocks.push_back(util::loadTexture(renderer, filenames[i]));
        }
    }
}