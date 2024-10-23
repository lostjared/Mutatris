#include"intro.hpp"
#include"util.hpp"
#include"puzzle.hpp"

namespace obj {


    void IntroObject::draw(SDL_Renderer *renderer) {
        SDL_RenderCopy(renderer, logo, nullptr, nullptr);
        util::printText(renderer, font, 25, 25, "[Press Enter to Play]", {255,255,255,255});
    }

    void IntroObject::event(SDL_Renderer *renderer, SDL_Event &e) {
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
            obj::setObject(new PuzzleObject());
            obj::object->load(renderer);
            return;
        }
    }

    void IntroObject::load(SDL_Renderer *renderer) {
        logo = util::loadTexture(renderer, "intro.png");
        font = util::loadFont("font.ttf", 54);
        std::cout << "Mutatris: Intro Init...\n";
    }

    IntroObject::~IntroObject() {
        std::cout << "Mutatris: Released Intro Object...\n";
        if(logo != nullptr) 
            SDL_DestroyTexture(logo);
        if(font != nullptr)
            TTF_CloseFont(font);
    }

}