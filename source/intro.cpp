#include"intro.hpp"
#include"util.hpp"
#include"puzzle.hpp"
#include<cstdlib>

#ifdef HAS_SOUND
#include"sound.hpp"
#endif

namespace obj {


    void IntroObject::draw(SDL_Renderer *renderer) {
        if(cur_screen == 0) {
            SDL_RenderCopy(renderer, logo, nullptr, nullptr);
            util::printText(renderer, font, 25, 25, "[Press Enter to Play]", {255,255,255,255});
            util::printText(renderer, large_font, 50, 250, "Mutatris", {255, 255, 255, 255});
            util::printText(renderer, font, 1280-500, 720-50, "lostsidedead.biz", {0,0,255,255});
        } else if(cur_screen == 1) {
            SDL_RenderCopy(renderer, start_screen, nullptr, nullptr);
            util::printText(renderer, medium_font, 560, 325, "Mutatris", {255, 255, 255, 255});
            util::printText(renderer, small_font, 600, 595, "Press Space",{255,255,255,255});
            SDL_Rect rc1 { 400, 450, 500, 100 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rc1);

            SDL_Color colors[] = {{255, 255, 255, 255}, {255,255,255,255}, {255,255,255,255}, {0,0,0,255}};

            colors[cursor_pos].r = static_cast<unsigned char>(rand()%255);
            colors[cursor_pos].g = static_cast<unsigned char>(rand()%255);
            colors[cursor_pos].b = static_cast<unsigned char>(rand()%255);

            util::printText(renderer, diff_font, 420, 475, "Easy", colors[0]);
            util::printText(renderer, diff_font, 580, 475, "Medium", colors[1]);
            util::printText(renderer, diff_font, 800, 475, "Hard", colors[2]);
        }
    }

    void IntroObject::event(SDL_Renderer *renderer, SDL_Event &e) {
        if(cur_screen == 0 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
            cur_screen = 1;
            return;
        }

        if(cur_screen == 1 && e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_LEFT:
                if(cursor_pos > 0) cursor_pos--;
                break;
                case SDLK_RIGHT:
                if(cursor_pos < 2) cursor_pos++;
                break;
                case SDLK_SPACE:
                obj::setObject(new PuzzleObject(cursor_pos));
                obj::object->load(renderer);
                break;
            }
        }
    }

    void IntroObject::load(SDL_Renderer *renderer) {
        logo = util::loadTexture(renderer, "intro.png");
        start_screen = util::loadTexture(renderer, "start.png");
        font = util::loadFont("font.ttf", 54);
        medium_font = util::loadFont("font.ttf", 44);
        large_font = util::loadFont("font.ttf", 275);
        small_font = util::loadFont("font.ttf", 20);
        diff_font = util::loadFont("font.ttf", 36);
        std::cout << "Mutatris: Intro Init...\n";
        #ifdef HAS_SOUND
        snd::playsound(1);
        #endif
    }

    IntroObject::~IntroObject() {
        std::cout << "Mutatris: Released Intro Object...\n";
        if(logo != nullptr) 
            SDL_DestroyTexture(logo);
        if(start_screen != nullptr) 
            SDL_DestroyTexture(start_screen);
        if(font != nullptr)
            TTF_CloseFont(font);
        if(large_font != nullptr)
            TTF_CloseFont(large_font);
        if(medium_font != nullptr) 
            TTF_CloseFont(medium_font);
        if(small_font != nullptr)
            TTF_CloseFont(small_font);
        if(diff_font != nullptr)
            TTF_CloseFont(diff_font);
    }

}