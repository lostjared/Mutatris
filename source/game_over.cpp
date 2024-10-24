#include"game_over.hpp"
#include"util.hpp"
#include<iostream>
#include"game_object.hpp"
#include"intro.hpp"

namespace obj {

    GameOverObject::GameOverObject(int scr, int clr) : score{scr}, clears{clr} {
        std::cout << "Mutatris: Game Over Initalized...\n";
    }

    GameOverObject::~GameOverObject() {

    }
        
    void GameOverObject::draw(SDL_Renderer *renderer) {
        SDL_RenderCopy(renderer, bg, nullptr, nullptr);
        util::printText(renderer, font, 25, 25, "Game Over Score: " + std::to_string(score) + " Clears: " + std::to_string(clears), {255,255,255,255});
        util::printText(renderer, font, 25, 100, "[ Press Space ]", {255, 255, 255, 255});
    }
    
    void GameOverObject::event(SDL_Renderer *renderer, SDL_Event &e) {
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            obj::setObject(new obj::IntroObject());
            obj::object->load(renderer);
        }
    }
        
    void GameOverObject::load(SDL_Renderer *renderer) {
        bg = util::loadTexture(renderer, "gameover.png");
        font = util::loadFont("font.ttf", 48);       
    }
        
}