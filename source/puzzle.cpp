#include "puzzle.hpp"
#include<iostream>
#include<string>

namespace obj {

    PuzzleObject::~PuzzleObject() {
        std::cout << "Mutatris: Game Released...\n";
    }

    void PuzzleObject::draw(SDL_Renderer *renderer) {

    }

    void PuzzleObject::event(SDL_Renderer *renderer, SDL_Event &e)  {

    }

    void PuzzleObject::load(SDL_Renderer *renderer) {
        std::cout << "Mutatris: Game Init...\n";
    }
}