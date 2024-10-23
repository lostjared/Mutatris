#include"util.hpp"

namespace util {

    #ifdef FOR_WASM
    std::string path = "/assets";
    #else
    std::string path = "assets";
    #endif

    std::string getFilePath(const std::string &filename) {
        std::ostringstream stream;
        stream << path << "/" << filename;
        return stream.str();
    }

    void printText(SDL_Renderer *renderer,TTF_Font *font,int x, int y, const std::string &text, SDL_Color col) {
        SDL_Surface *surf = TTF_RenderText_Blended(font,text.c_str(), col);
        if(!surf) {
            std::cerr << "Error rendering text...\n";
            return;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        if(!tex) {
            std::cerr << "Error creating texture..\n";
            SDL_FreeSurface(surf);
            return;
        }
        SDL_Rect rc = {x,y,surf->w,surf->h};
        SDL_RenderCopy(renderer, tex, nullptr, &rc);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }
}