#include"startup.hpp"
#include"util.hpp"
#include"puzzle.hpp"
#include<cstdlib>
#include"intro.hpp"
#ifdef HAS_SOUND
#include"sound.hpp"
#endif

namespace obj {


    void StartupObject::draw(SDL_Renderer *renderer) {
        if(cur_screen == 0) {
            SDL_RenderCopy(renderer, logo, nullptr, nullptr);
        } else if(cur_screen == 1) {
            SDL_RenderCopy(renderer, jb_logo, nullptr, nullptr);
        } else if(cur_screen == 2) {
            SDL_RenderCopy(renderer, jb_logo, nullptr, nullptr);
            obj::setObject(new obj::IntroObject());
            obj::object->load(renderer);
        }

        static Uint32 previous_time = SDL_GetTicks();
        Uint32 current_time = SDL_GetTicks();
        if (current_time - previous_time >= 3000) {
            cur_screen ++;
            previous_time = current_time;
        }
    }

    void StartupObject::event(SDL_Renderer *renderer, SDL_Event &e) {

    }

    void StartupObject::load(SDL_Renderer *renderer) {
        logo = util::loadTexture(renderer, "lostlogo.png");
        jb_logo = util::loadTexture(renderer, "jblogo.png");
    }

    StartupObject::~StartupObject() {
        std::cout << "Mutatris: Releasing Startup Object..\n";
        if(logo) 
            SDL_DestroyTexture(logo);
        if(jb_logo) 
            SDL_DestroyTexture(jb_logo);

    }

}