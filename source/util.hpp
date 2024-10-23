#ifndef __UTIL_H__
#define __UTIL_H__

#include<iostream>
#include<sstream>
#include<string>
#include"SDL.h"
#include"SDL_ttf.h"

namespace util  {

extern std::string path;
std::string getFilePath(const std::string &filename);
void printText(SDL_Renderer *renderer,TTF_Font *font, int x, int y, const std::string &text, SDL_Color col);
SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filename);
SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filename, int &w, int &h, bool color, SDL_Color key);

}

#endif