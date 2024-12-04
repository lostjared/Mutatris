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
        if(bg)
            SDL_DestroyTexture(bg);
        if(bg_score)
            SDL_DestroyTexture(bg_score);
        if(font)
            TTF_CloseFont(font);
    }
    void GameOverObject::draw(SDL_Renderer *renderer) {               
        if(score < 200) {
            SDL_RenderCopy(renderer, bg, nullptr, nullptr);
            util::printText(renderer, font, 25, 25, "Game Over Score: " + std::to_string(score) + " Clears: " + std::to_string(clears), {255,255,255,255});
            util::printText(renderer, font, 25, 100, "[ Press Space ]", {255, 255, 255, 255});
        } else {
            SDL_RenderCopy(renderer, bg_score, nullptr, nullptr);
            util::printText(renderer, font, 25, 25, "High Score: " + std::to_string(score) + " Cleared: " + std::to_string(clears), {255,255,255,255});
            util::printText(renderer, font, 25, 100, "Thanks for Playing! ", {255,255,255,255});
            util::printText(renderer, font, 25, 600, "Programming by Jared Bruni", {255, 255, 255, 255});
            util::printText(renderer, font, 25, 650, "Sound by Unknown", {255,255,255,255});
            util::printText(renderer, font, 25, 400, "[Press Space]", {255,255,255,255});
        }
    }
    
    void GameOverObject::event(SDL_Renderer *renderer, SDL_Event &e) {
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            obj::setObject(new obj::IntroObject());
            obj::object->load(renderer);
            return;
        } else if(e.type == SDL_JOYBUTTONDOWN && e.jbutton.button == 0) {
            obj::setObject(new obj::IntroObject());
            obj::object->load(renderer);
            return;
        } else if((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_FINGERDOWN)) {
            obj::setObject(new obj::IntroObject());
            obj::object->load(renderer);
            return;
        }
    }
        
    void GameOverObject::load(SDL_Renderer *renderer) {
        bg = util::loadTexture(renderer, "gameover.png");
        bg_score = util::loadTexture(renderer, "highscore.png");
        font = util::loadFont("font.ttf", 48);       
    }
        
}