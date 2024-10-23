#include"intro.hpp"
#include"util.hpp"

namespace obj {


    void IntroObject::draw(SDL_Renderer *renderer) {
        SDL_RenderCopy(renderer, logo, nullptr, nullptr);
    }

    void IntroObject::event(SDL_Event &e) {

    }

    void IntroObject::load(SDL_Renderer *renderer) {
        logo = util::loadTexture(renderer, "intro.png");
    }

    IntroObject::~IntroObject() {
        if(logo != nullptr)
            SDL_DestroyTexture(logo);
    }

}