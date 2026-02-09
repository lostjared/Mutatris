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
            static Uint32 previous_time = SDL_GetTicks();
            Uint32 current_time = SDL_GetTicks();
            static int alpha = 255;
            static bool fading_out = true;

            if (cur_screen == 0) {
                SDL_SetTextureAlphaMod(logo, alpha);
                SDL_RenderCopy(renderer, logo, nullptr, nullptr);
            } else if (cur_screen == 1) {
                SDL_SetTextureAlphaMod(jb_logo, alpha);
                SDL_RenderCopy(renderer, jb_logo, nullptr, nullptr);
            } else if (cur_screen == 2) {
                SDL_SetTextureAlphaMod(jb_logo, alpha);
                SDL_RenderCopy(renderer, jb_logo, nullptr, nullptr);
                obj::setObject(new obj::IntroObject());
                obj::object->load(renderer);
            }

            if (current_time - previous_time >= 30) {
                previous_time = current_time;
                if (fading_out) {
                    alpha -= 3;  
                    if (alpha <= 0) {
                        alpha = 0;
                        fading_out = false;
                        cur_screen++;
                    }
                } else {
                    alpha += 3;
                    if (alpha >= 255) {
                        alpha = 255;
                        fading_out = true;
                        previous_time = SDL_GetTicks();
                    }
                }
            }
        }


    void StartupObject::event(SDL_Renderer *renderer, SDL_Event &e) {
       // skip intro
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                obj::setObject(new obj::IntroObject());
                obj::object->load(renderer);
        }
        if(e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                obj::setObject(new obj::IntroObject());
                obj::object->load(renderer);
        }
        
        util::connectJoystick(e);
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