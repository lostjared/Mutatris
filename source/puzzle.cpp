#include "puzzle.hpp"
#include<iostream>
#include<string>
#include"util.hpp"

namespace obj {

    PuzzleObject::~PuzzleObject() {
        std::cout << "Mutatris: Game Released...\n";
        for(int i = 0; i < blocks.size(); ++i) {
            SDL_DestroyTexture(blocks[i]);
        }
        for(int i = 0; i < backgrounds.size(); ++i) {
            SDL_DestroyTexture(backgrounds[i]);
        }
    }

    void PuzzleObject::draw(SDL_Renderer *renderer) {
        SDL_RenderCopy(renderer, backgrounds[cur_level], nullptr, nullptr);
        int grid_size_x = 32 * game.grid[0].width();  
        int window_size_x = 1280;  
        int window_size_y = 720;   
        int x_loc = (window_size_x / 2) - (grid_size_x / 2);  
        for (int i = 0; i < game.grid[0].width(); ++i) {
            for (int z = 0; z < game.grid[0].height(); ++z) {
                int pos_x = x_loc + 32 * i;
                int pos_y = 16 * z;
                SDL_Rect rc = { pos_x, pos_y, 30, 14 };
                puzzle::Block *b = game.grid[0].at(i, z);
                if(b != nullptr)
                    SDL_RenderCopy(renderer, blocks[b->color], nullptr, &rc);
            }
        }
        int grid_size_y = 16 * game.grid[1].height();
        int y_loc = window_size_y - grid_size_y;  
        for (int i = 0; i < game.grid[1].width(); ++i) {
            for (int z = 0; z < game.grid[1].height(); ++z) {
                int pos_x = x_loc + 32 * i;
                int pos_y = y_loc + 16 * z;
                puzzle::Block *b = game.grid[1].at(i, game.grid[1].height()-z-1);
                SDL_Rect rc = { pos_x, pos_y, 30, 14 };
                if(b != nullptr) 
                    SDL_RenderCopy(renderer, blocks[b->color], nullptr, &rc);
            }
        }
        int left_x_loc = 0;  
        int left_y_loc = (window_size_y / 2) - (grid_size_y / 2);  
        for (int z = 0; z < game.grid[2].height(); ++z) {
            for (int i = 0; i < game.grid[2].width(); ++i) {
                int pos_x = left_x_loc + 16 * z;  
                int pos_y = left_y_loc + 32 * i;  
                SDL_Rect rc = { pos_x, pos_y, 30, 14 }; 
                puzzle::Block *b = game.grid[2].at(game.grid[2].width()-i-1, game.grid[2].height()-z-1);
                if(b != nullptr) 
                    SDL_RenderCopyEx(renderer, blocks[b->color], nullptr, &rc, 90.0, nullptr, SDL_FLIP_NONE);  
            }
        }
        int right_x_loc = window_size_x - 16;  
        int right_y_loc = (window_size_y / 2) - (grid_size_y / 2);  
        for (int z = 0; z < game.grid[3].height(); ++z) {
            for (int i = 0; i < game.grid[3].width(); ++i) {
                int pos_x = right_x_loc - 16 * z;  
                int pos_y = right_y_loc + 32 * i;  
                SDL_Rect rc = { pos_x, pos_y, 30, 14 }; 
                puzzle::Block *b = game.grid[3].at(game.grid[3].width()-i-1, game.grid[3].height()-z-1);
                if(b != nullptr)
                    SDL_RenderCopyEx(renderer, blocks[b->color], nullptr, &rc, -90.0, nullptr, SDL_FLIP_NONE); 
            }
        }
    }

    void PuzzleObject::event(SDL_Renderer *renderer, SDL_Event &e)  {

    }

    void PuzzleObject::load(SDL_Renderer *renderer) {
        std::cout << "Mutatris: Game Init...\n";
        static const char *filenames[] = { "block_black.png", "block_clear.png", "block_dblue.png", "block_green.png", "block_ltblue.png", "block_orange.png", "block_red.png", "block_yellow.png", 0};
        for(int i = 0; filenames[i] != 0; ++i) {
            blocks.push_back(util::loadTexture(renderer, filenames[i]));
        }
        static const char *bg_filenames[] = {"blocks.png", 0};
        for(int i = 0; bg_filenames[i] != 0; ++i) {
            backgrounds.push_back(util::loadTexture(renderer, bg_filenames[i]));
        }
    }
}