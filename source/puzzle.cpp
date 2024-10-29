#include "puzzle.hpp"
#include<iostream>
#include<string>
#include<random>
#include<algorithm>
#include"util.hpp"
#include"game_over.hpp"

namespace obj {

    PuzzleObject::~PuzzleObject() {
        std::cout << "Mutatris: Game Released...\n";
        for(size_t i = 0; i < blocks.size(); ++i) {
            SDL_DestroyTexture(blocks[i]);
        }
        for(size_t i = 0; i < backgrounds.size(); ++i) {
            SDL_DestroyTexture(backgrounds[i]);
        }
        for(size_t i = 0; i <= 3;  ++i) {
            SDL_DestroyTexture(game_textures[i]);
        }

        if(font)
            TTF_CloseFont(font);
    }

    void PuzzleObject::drawGrid(SDL_Renderer *renderer, SDL_Texture *texture, int focus) {
              
        SDL_Texture* old_target = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderCopy(renderer, backgrounds[game.level < static_cast<int>(backgrounds.size()) ? game.level : backgrounds.size()-1], nullptr, nullptr);
        for (int i = 0; i < game.grid[focus].width(); ++i) {
            for (int z = 0; z < game.grid[focus].height(); ++z) {
                int pos_x = 32 * i;
                int pos_y = 16 * z;
                SDL_Rect rc = { pos_x, pos_y, 30, 14 };
                puzzle::Block *b = game.grid[focus].at(i, z);
                if(b != nullptr) {
                    if(b->color >= 0)
                        SDL_RenderCopy(renderer, blocks[b->color], nullptr, &rc);
                    else {
                        SDL_RenderCopy(renderer, blocks[1+(rand()%6)], nullptr, &rc);      
                    }
                    
                }
            }
        }

        
        if(cur_focus == focus) {
            int x_val = game.grid[focus].game_piece.getX();
            int y_val = game.grid[focus].game_piece.getY();  
            for(int q = 0; q < 3; ++q) {
                int cx = 32 * x_val;
                int cy = 16 * y_val + (16 * q);
                switch(game.grid[focus].game_piece.getDirection()) {
                    case 1:
                    cx = 32 * (x_val + q);
                    cy = (16 * y_val);
                    break;
                    case 2:
                    cx = 32 * (x_val - q);
                    cy = (16 * y_val);
                    break;
                    case 3:
                    cx = 32 * x_val;
                    cy = (16 * y_val) - (16 * q);
                    break;
                }
                SDL_Rect pos_rc = { cx, cy, 32, 16 };
                puzzle::Block *b = game.grid[focus].game_piece.at(q);
                if(b != nullptr)
                    SDL_RenderCopy(renderer, blocks[b->color], nullptr, &pos_rc);
            }
        }
        SDL_SetRenderTarget(renderer, old_target);
        game.procBlocks();
    }

    void PuzzleObject::twistColors() {
        static Uint32 previous_time = SDL_GetTicks();
        Uint32 current_time = SDL_GetTicks();
        Uint32 delta_time = current_time - previous_time;
        if (delta_time >= 25) {        
            for(int focus = 0; focus <= 3; focus ++) {
                for (int i = 0; i < game.grid[focus].width(); ++i) {
                    for (int z = 0; z < game.grid[focus].height(); ++z) {
                        puzzle::Block *b = game.grid[focus].at(i, z);
                        if (b && b->color < 0) {
                            b->color--;
                            if (b->color < -60) {
                                b->color = 0;
                            }
                        }
                    }
                }
            }
            previous_time += delta_time; 
        }
    }


    void PuzzleObject::draw(SDL_Renderer *renderer) {
        SDL_RenderCopy(renderer, backgrounds[game.level < static_cast<int>(backgrounds.size()) ? game.level : backgrounds.size()-1], nullptr, nullptr);
        drawGrid(renderer, game_textures[0], 0);
        int width = 0, height = 0;
        SDL_QueryTexture(game_textures[0], nullptr, nullptr, &width, &height);
        int window_x = (1280/2) - (width/2);
        SDL_Rect rc = {window_x, 0, width, height};
        SDL_RenderCopy(renderer, game_textures[0], nullptr, &rc);

        drawGrid(renderer, game_textures[1], 1);
        SDL_QueryTexture(game_textures[1], nullptr, nullptr, &width, &height);
        window_x = 32;
        int window_y = (720/2) - (height/2);
        SDL_Rect rc2 = { window_x, window_y, width, height };
        SDL_RenderCopyEx(renderer, game_textures[1], nullptr, &rc2, -90.0, nullptr, SDL_FLIP_NONE);  
        
        drawGrid(renderer, game_textures[2], 2);
        SDL_QueryTexture(game_textures[2], nullptr, nullptr, &width, &height);
        window_x = (1280/2) - (width/2);
        window_y = (720/2)+5;
        SDL_Rect rc3 = { window_x, window_y, width, height };
        SDL_RenderCopyEx(renderer, game_textures[2], nullptr, &rc3, 0, nullptr, SDL_FLIP_VERTICAL);  
        
        drawGrid(renderer, game_textures[3], 3);
        SDL_QueryTexture(game_textures[3], nullptr, nullptr, &width, &height);
        window_x = (1280)-width -32;
        window_y = (720/2)-(height/2);
        SDL_Rect rc4 = { window_x, window_y, width, height };
        SDL_RenderCopyEx(renderer, game_textures[3], nullptr, &rc4, 90.0, nullptr, SDL_FLIP_HORIZONTAL);  

        util::printText(renderer, font, 25, 25, "Score: " + std::to_string(game.score), {255,255,255,255});
        util::printText(renderer, font, 25, 55, "Direction: " + std::to_string(cur_focus), {255, 255, 255, 255});
        util::printText(renderer, font, 25, 85, "Level: " + std::to_string(game.level+1) + " Timeout: " + std::to_string(game.timeout), {255, 255, 255, 255});
        
        twistColors();

        static Uint32 previous_time = SDL_GetTicks();
        Uint32 current_time = SDL_GetTicks();
        if (current_time - previous_time >= game.timeout) {
            if(game.grid[cur_focus].canMoveDown()) {
                game.grid[cur_focus].game_piece.moveDown();
                previous_time = current_time;
            } else {
                obj::setObject(new obj::GameOverObject(game.score, game.clears));
                obj::object->load(renderer);
            }
        }
    }

    void PuzzleObject::event(SDL_Renderer *renderer, SDL_Event &e)  {

        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s) {
            game.grid[cur_focus].game_piece.drop();
            return;
        }

        switch(e.type) {
            case SDL_KEYDOWN: {
                if(cur_focus == 0) {
                    switch(e.key.keysym.sym) {
                        case SDLK_LEFT:
                            game.grid[cur_focus].game_piece.moveLeft();
                        break;
                        case SDLK_RIGHT:
                            game.grid[cur_focus].game_piece.moveRight(); 
                        break;
                        case SDLK_UP:
                            game.grid[cur_focus].game_piece.shiftColors(); 
                        break;
                        case SDLK_DOWN:
                            game.grid[cur_focus].game_piece.moveDown();
                        break;
                        case SDLK_a:
                        case SDLK_SPACE:
                            game.grid[cur_focus].game_piece.shiftDirection();
                        break;
                        default:
                        break;
                    }
                } else if(cur_focus == 1) {
                    switch(e.key.keysym.sym) {
                        case SDLK_DOWN:
                            game.grid[cur_focus].game_piece.moveLeft();
                        break;
                        case SDLK_UP:
                            game.grid[cur_focus].game_piece.moveRight();     
                        break;
                        case SDLK_LEFT:
                            game.grid[cur_focus].game_piece.shiftColors(); 
                        break;
                        case SDLK_RIGHT:
                            game.grid[cur_focus].game_piece.moveDown();
                        break;
                        case SDLK_a:
                        case SDLK_SPACE:
                            game.grid[cur_focus].game_piece.shiftDirection();
                        break;
                        default:
                        break;
                    }

                } else if(cur_focus == 2) {
                    switch(e.key.keysym.sym) {
                        case SDLK_DOWN:
                            game.grid[cur_focus].game_piece.shiftColors();
                        break;
                        case SDLK_UP:
                            game.grid[cur_focus].game_piece.moveDown();     
                        break;
                        case SDLK_LEFT:
                            game.grid[cur_focus].game_piece.moveLeft(); 
                        break;
                        case SDLK_RIGHT:
                            game.grid[cur_focus].game_piece.moveRight();
                        break;
                        case SDLK_a:
                        case SDLK_SPACE:
                            game.grid[cur_focus].game_piece.shiftDirection();
                        break;
                        default:
                        break;
                    }
                } else if(cur_focus == 3) {
                    switch(e.key.keysym.sym) {
                        case SDLK_DOWN:
                            game.grid[cur_focus].game_piece.moveLeft();
                        break;
                        case SDLK_UP:
                            game.grid[cur_focus].game_piece.moveRight();     
                        break;
                        case SDLK_LEFT:
                            game.grid[cur_focus].game_piece.moveDown(); 
                        break;
                        case SDLK_RIGHT:
                            game.grid[cur_focus].game_piece.shiftColors();
                        break;
                        case SDLK_a:
                        case SDLK_SPACE:
                            game.grid[cur_focus].game_piece.shiftDirection();
                        break;
                        default:
                        break;
                    }
                }
            }
            break;
        }
    }

    void PuzzleObject::load(SDL_Renderer *renderer) {
        std::cout << "Mutatris: Game Init...\n";
        static const char *filenames[] = { "block_black.png", "block_clear.png", "block_dblue.png", "block_green.png", "block_ltblue.png", "block_orange.png", "block_red.png", "block_yellow.png", 0};
        for(int i = 0; filenames[i] != 0; ++i) {
            blocks.push_back(util::loadTexture(renderer, filenames[i]));
        }
        static const char *bg_filenames[] = {"blocks.png","blocks1.png","blocks2.png","blocks3.png", "blocks4.png", "blocks5.png", "blocks6.png", "blocks7.png", "blocks8.png", "blocks9.png", "blocks10.png", 0};
        for(int i = 0; bg_filenames[i] != 0; ++i) {
            backgrounds.push_back(util::loadTexture(renderer, bg_filenames[i]));
        }
        std::random_device rd;
        std::mt19937 rng(rd()); 
        std::shuffle(backgrounds.begin(), backgrounds.end(), rng);
        game.setCallback([&]() -> void {
            cur_focus++;
            if(cur_focus > 3)
                cur_focus = 0;
        });
        for(int i = 0; i <= 3; ++i) {
            game_textures[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 32 * game.grid[i].width(), 16 * game.grid[i].height());
            if(!game_textures[i]) {
                std::cerr << "Mutatris: Error creating grid texture..\n";
                std::cerr.flush();
                exit(EXIT_FAILURE);
            }
        }
        font = util::loadFont("font.ttf", 32);
    }
}