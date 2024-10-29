#include"sound.hpp"

#ifdef HAS_SOUND
#include"SDL_mixer.h"
#include"util.hpp"
#include<iostream>
#include<vector>

namespace snd {

    std::vector<Mix_Chunk *> muz;

    void load_sound(const std::string &s) {
        Mix_Chunk *c = Mix_LoadWAV(util::getFilePath(s).c_str());
        if(!c) {
            std::cerr << "Mutatris: Could not open sound file..\n" << Mix_GetError() << "\n";
            std::cerr.flush();
            std::exit(EXIT_FAILURE);
        }
        muz.push_back(c);
    }

    void init() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
            std::cerr << "Error initalzing audio device...\n";
            std::cerr.flush();
            std::exit(EXIT_FAILURE);
        }
        
        load_sound("line.wav");
        load_sound("open.wav");
    }


    void playsound(int id) {
        if(id >= 0 && id < static_cast<int>(muz.size())) {
            Mix_PlayChannel(-1, muz[id], 0);
            std::cout << "Mutatris: Played sound effect: " << id << "\n";
        }
    }

    void release() {
        for(auto &i : muz) {
            Mix_FreeChunk(i);
        }
        Mix_CloseAudio();
    }

}


#endif