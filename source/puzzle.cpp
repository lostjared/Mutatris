#include "puzzle.hpp"
#include<iostream>
#include<string>
#include<random>
#include<algorithm>
#include"util.hpp"
#include"game_over.hpp"

namespace obj {


    PuzzleObject::PuzzleObject(int diff) : game{diff} {}

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

        if(pause_menu)
            SDL_DestroyTexture(pause_menu);


        if(font)
            TTF_CloseFont(font);
        
        if(paused_small)
            TTF_CloseFont(paused_small);

        if(paused_large)
            TTF_CloseFont(paused_large);
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

        if(paused) {
            SDL_RenderCopy(renderer, pause_menu, nullptr, nullptr);
            util::printText(renderer, paused_large, 580, 325, "Paused", {255, 255, 255, 255});
            util::printText(renderer, paused_small, 600, 595, "Press Space",{255,255,255,255});
            return;
        }

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

        util::printText(renderer, font, 25, 25, "Level: " + std::to_string(game.level+1) + " Timeout: " + std::to_string(game.timeout), {255, 255, 255, 255});
        util::printText(renderer, font, 25, 55, "Score: " + std::to_string(game.score), {255,255,255,255});
        util::printText(renderer, font, 25, 85, "Direction: " + std::to_string(cur_focus), {255, 255, 255, 255});
        
        
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
                return;
            }
        }

        for(auto &c : util::stick) {
            handleControllerButtonRepeat(c, cur_focus, current_time);
        }

        if (current_time - last_stick_time >= STICK_REPEAT_INTERVAL) {
            for (auto &c : util::stick) {
                if (c) {
                    int axis_x = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX);
                    int axis_y = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);
                    if (abs(axis_x) > 8000 || abs(axis_y) > 8000) {
                        procStick(cur_focus, axis_x, axis_y);
                        last_stick_time = current_time;
                    }
                }
            }
        }
    }

    void PuzzleObject::handleControllerButtonRepeat(SDL_GameController *controller, int cur_focus, Uint32 current_time) {
        for (auto &entry : button_states) {
            SDL_GameControllerButton button = entry.first;
            ButtonState &state = entry.second;

            if (state.is_pressed && current_time >= state.last_repeat_time) {
                if (current_time - state.last_repeat_time >= REPEAT_INTERVAL) {
                    procDPad(cur_focus, button);
                    state.last_repeat_time = current_time;
                }
            }
        }
    }

    void PuzzleObject::procStick(int cur_focus, int x_axis, int y_axis) {
        const int DEADZONE = 8000;
        if (cur_focus == 0) {
            if (x_axis < -DEADZONE) {
                game.grid[cur_focus].game_piece.moveLeft();
            } else if (x_axis > DEADZONE) {
                game.grid[cur_focus].game_piece.moveRight();
            }
            if (y_axis > DEADZONE) {
                game.grid[cur_focus].game_piece.moveDown();
            }
        } else if (cur_focus == 1) {
            if (y_axis > DEADZONE) {
                game.grid[cur_focus].game_piece.moveLeft();
            } else if (y_axis < -DEADZONE) {
                game.grid[cur_focus].game_piece.moveRight();
            }
            if (x_axis > DEADZONE) {
                game.grid[cur_focus].game_piece.moveDown();
            }
        } else if (cur_focus == 2) {
            if (x_axis > DEADZONE) {
                game.grid[cur_focus].game_piece.moveRight();
            } else if (x_axis < -DEADZONE) {
                game.grid[cur_focus].game_piece.moveLeft();
            }

            if (y_axis < -DEADZONE) {
                game.grid[cur_focus].game_piece.moveDown();
            }
        } else if (cur_focus == 3) {
            if (y_axis > DEADZONE) {
                game.grid[cur_focus].game_piece.moveLeft();
            } else if (y_axis < -DEADZONE) {
                game.grid[cur_focus].game_piece.moveRight();
            }

            if (x_axis < -DEADZONE) {
                game.grid[cur_focus].game_piece.moveDown();
            }
        }
    }

    void PuzzleObject::procDPad(int cur_focus, int button) {
        switch (cur_focus) {
            case 0:
                if (button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
                    game.grid[cur_focus].game_piece.moveLeft();
                } else if (button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
                    game.grid[cur_focus].game_piece.moveRight();
                }
                if (button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                    game.grid[cur_focus].game_piece.moveDown();
                }
                break;
            case 1:
                if (button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                    game.grid[cur_focus].game_piece.moveLeft();
                } else if (button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                    game.grid[cur_focus].game_piece.moveRight();
                }
                if (button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
                    game.grid[cur_focus].game_piece.moveDown();
                }
                break;
            case 2:
                if (button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
                    game.grid[cur_focus].game_piece.moveRight();
                } else if (button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
                    game.grid[cur_focus].game_piece.moveLeft();
                }
                if (button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                    game.grid[cur_focus].game_piece.moveDown();
                }
                break;
            case 3:
                if (button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                    game.grid[cur_focus].game_piece.moveLeft();
                } else if (button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                    game.grid[cur_focus].game_piece.moveRight();
                }
                if (button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
                    game.grid[cur_focus].game_piece.moveDown();
                }
                break;
            default:
                break;
        }

        switch(button) {
            case SDL_CONTROLLER_BUTTON_A:
            game.grid[cur_focus].game_piece.shiftColors();
            break;
            case SDL_CONTROLLER_BUTTON_B:
            game.grid[cur_focus].game_piece.shiftDirection();
            break;
            case SDL_CONTROLLER_BUTTON_Y:
            game.grid[cur_focus].game_piece.drop();
            break;
        }
    }

    void PuzzleObject::procHat(int cur_focus, int value) {
        if(cur_focus == 0) {
                switch(value) {
                    case 8:
                        game.grid[cur_focus].game_piece.moveLeft();
                    break;
                    case 2:
                        game.grid[cur_focus].game_piece.moveRight(); 
                    break;
                    case 4:
                        game.grid[cur_focus].game_piece.moveDown();
                    break;
                    default:
                    break;
                }

            } else if(cur_focus == 1) {
                switch(value) {
                    case 4:
                        game.grid[cur_focus].game_piece.moveLeft();
                    break;
                    case 1:
                        game.grid[cur_focus].game_piece.moveRight();     
                    break;
                    case 2:
                        game.grid[cur_focus].game_piece.moveDown();
                    break;
                    default:
                    break;
                }

            } else if(cur_focus == 2) {
                switch(value) {
                    case 1:
                        game.grid[cur_focus].game_piece.moveDown();     
                    break;
                    case 8:
                        game.grid[cur_focus].game_piece.moveLeft(); 
                    break;
                    case 2:
                        game.grid[cur_focus].game_piece.moveRight();
                    break;
                    default:
                    break;
                }
            } else if(cur_focus == 3) {
                switch(value) {
                    case 4:
                        game.grid[cur_focus].game_piece.moveLeft();
                    break;
                    case 1:
                        game.grid[cur_focus].game_piece.moveRight();     
                    break;
                    case 8:
                        game.grid[cur_focus].game_piece.moveDown(); 
                    break;
                    default:
                    break;
                }
            }
    }

    void PuzzleObject::event(SDL_Renderer *renderer, SDL_Event &e) {

        util::connectJoystick(e);

        static bool isDragging = false;
        static SDL_Point lastMousePos = {0, 0};
        static int screenWidth = 1280;
        static int screenHeight = 720;

        if (!paused) {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
                paused = true;
                return;
            }
            if (e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                paused = true;
                return;
            }
        } else {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                paused = false;
            }
            if (e.type == SDL_CONTROLLERBUTTONDOWN && 
                (e.cbutton.button == SDL_CONTROLLER_BUTTON_START || e.cbutton.button == SDL_CONTROLLER_BUTTON_A)) {
                paused = false;
            }
            return;
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s) {
            game.grid[cur_focus].game_piece.drop();
            return;
        }

        Uint32 current_time = SDL_GetTicks();

        switch (e.type) {
            case SDL_CONTROLLERBUTTONDOWN: {
                SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(e.cbutton.button);
                button_states[button] = { true, current_time + REPEAT_DELAY };
                if (button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
                    cur_focus++;
                    if (cur_focus > 3) cur_focus = 0;
                    break;
                }
                if (button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) {
                    cur_focus--;
                    if (cur_focus < 0) cur_focus = 3;
                    break;
                }
                procDPad(cur_focus, button);
                break;
            }
            case SDL_CONTROLLERBUTTONUP: {
                SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(e.cbutton.button);
                button_states[button].is_pressed = false;
                break;
            }
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_w) {
                        game.grid[cur_focus].game_piece.shiftColors();
                        return;
                    }

                    if (cur_focus == 0) {
                        switch (e.key.keysym.sym) {
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
                    } else if (cur_focus == 1) {
                        switch (e.key.keysym.sym) {
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
                    } else if (cur_focus == 2) {
                        switch (e.key.keysym.sym) {
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
                    } else if (cur_focus == 3) {
                        switch (e.key.keysym.sym) {
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
                break;
            case SDL_MOUSEBUTTONDOWN: {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    isDragging = true;
                    Uint32 currentTime = SDL_GetTicks();
                    int x = e.button.x;
                    int y = e.button.y;
                    if (currentTime - lastTapTime > DOUBLE_TAP_THRESHOLD) {
                        lastTapTime = 0;
                    }
                    if (currentTime - lastTapTime <= DOUBLE_TAP_THRESHOLD &&
                        abs(x - lastTapPos.x) <= DOUBLE_TAP_DISTANCE &&
                        abs(y - lastTapPos.y) <= DOUBLE_TAP_DISTANCE) {
                        handleDoubleTap({x, y});
                        lastTapTime = 0;
                    } else {
                        lastTapTime = currentTime;
                        lastTapPos = {x, y};
                    }

                    lastMousePos = { e.button.x, e.button.y };
                    swipeStartPos = lastMousePos;
                    swipeStartTime = SDL_GetTicks();
                    moveBlockToPosition(lastMousePos);
                }
                break;
            }

            case SDL_MOUSEBUTTONUP: {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                    SDL_Point mouseEndPos = { e.button.x, e.button.y };
                    Uint32 swipeEndTime = SDL_GetTicks();
                    detectSwipe(swipeStartPos, mouseEndPos, swipeStartTime, swipeEndTime);
                }
                break;
            }

            case SDL_MOUSEMOTION: {
                if (isDragging) {
                    SDL_Point currentMousePos = { e.motion.x, e.motion.y };
                    moveBlockToPosition(currentMousePos);
                    lastMousePos = currentMousePos;
                }
                break;
            }
            case SDL_FINGERDOWN: {
                int x = static_cast<int>(e.tfinger.x * screenWidth);
                int y = static_cast<int>(e.tfinger.y * screenHeight);
                Uint32 currentTime = SDL_GetTicks();

                if (isTapPending) {
                    if (currentTime - lastTapTime <= DOUBLE_TAP_THRESHOLD &&
                        abs(x - lastTapPos.x) <= DOUBLE_TAP_DISTANCE &&
                        abs(y - lastTapPos.y) <= DOUBLE_TAP_DISTANCE) {
                        handleDoubleTap({x, y});
                        isTapPending = false;
                        lastTapTime = 0;
                    } else {
                        handleSingleTap({lastTapPos.x, lastTapPos.y});
                        isTapPending = false;
                        lastTapTime = currentTime;
                        lastTapPos = {x, y};
                    }
                } else {
                    isTapPending = true;
                    lastTapTime = currentTime;
                    lastTapPos = {x, y};
                    SDL_AddTimer(DOUBLE_TAP_THRESHOLD, [](Uint32 interval, void* param) -> Uint32 {
                        PuzzleObject* self = static_cast<PuzzleObject*>(param);
                        if (self->isTapPending) {
                            self->handleSingleTap(self->lastTapPos);
                            self->isTapPending = false;
                        }
                        return 0; 
                    }, this);
                }
                break;
            }
            case SDL_FINGERUP: {
                if (isDragging) {
                    isDragging = false;
                    int x = static_cast<int>(e.tfinger.x * screenWidth);
                    int y = static_cast<int>(e.tfinger.y * screenHeight);
                    SDL_Point touchEndPos = { x, y };
                    Uint32 swipeEndTime = SDL_GetTicks();
                    detectSwipe(swipeStartPos, touchEndPos, swipeStartTime, swipeEndTime);
                }
            }
                break;

            case SDL_FINGERMOTION: {
                isDragging = true;
                int x = static_cast<int>(e.tfinger.x * screenWidth);
                int y = static_cast<int>(e.tfinger.y * screenHeight);
                lastMousePos = { x, y };
                moveBlockToPosition(lastMousePos);
            }
            break;

            case SDL_JOYHATMOTION: {
                if (!SDL_GameControllerFromInstanceID(e.jhat.which)) {
                    procHat(cur_focus, e.jhat.value);
                }
                break;
            }

            case SDL_JOYAXISMOTION: {
                if (!SDL_IsGameController(e.jaxis.which)) {
                    if (e.jaxis.axis == 0) joy_axis_x = e.jaxis.value;
                    if (e.jaxis.axis == 1) joy_axis_y = e.jaxis.value;
                    Uint32 ct = SDL_GetTicks();
                    if (ct - last_stick_time >= STICK_REPEAT_INTERVAL) {
                        if (abs(joy_axis_x) > 8000 || abs(joy_axis_y) > 8000) {
                            procStick(cur_focus, joy_axis_x, joy_axis_y);
                            last_stick_time = ct;
                        }
                    }
                }
                break;
            }

            case SDL_JOYBUTTONDOWN: {
                if (!SDL_IsGameController(e.jbutton.which)) {
                    switch (e.jbutton.button) {
                        case 0:
                            game.grid[cur_focus].game_piece.shiftColors();
                            break;
                        case 1:
                            game.grid[cur_focus].game_piece.shiftDirection();
                            break;
                        case 2:
                            game.grid[cur_focus].game_piece.drop();
                            break;
                        case 4:
                            cur_focus++;
                            if (cur_focus > 3) cur_focus = 0;
                            break;
                        case 5:
                            cur_focus--;
                            if (cur_focus < 0) cur_focus = 3;
                            break;
                        default:
                            break;
                    }
                }
                break;
            }

            default:
                break;
        }
    }

    void PuzzleObject::detectSwipe(SDL_Point startPos, SDL_Point endPos, Uint32 startTime, Uint32 endTime) {
        int deltaX = endPos.x - startPos.x;
        int deltaY = endPos.y - startPos.y;
        Uint32 deltaTime = endTime - startTime;
 
        if (deltaTime > SWIPE_TIME_THRESHOLD) {
            return;
        }

        if (abs(deltaX) < SWIPE_DISTANCE_THRESHOLD && abs(deltaY) < SWIPE_DISTANCE_THRESHOLD) {
            return;
        }

        SwipeDirection swipeDir = SWIPE_NONE;
        if (abs(deltaX) > abs(deltaY)) {
            swipeDir = (deltaX > 0) ? SWIPE_RIGHT : SWIPE_LEFT;
        } else {
            swipeDir = (deltaY > 0) ? SWIPE_DOWN : SWIPE_UP;
        }
        if (isDropSwipe(swipeDir)) {
            game.grid[cur_focus].game_piece.drop();
        }
        if(isSwitchSwipe(swipeDir)) {
            game.grid[cur_focus].game_piece.shiftColors();
        }
    }

    void PuzzleObject::handleDoubleTap(SDL_Point p) {
        game.grid[cur_focus].game_piece.shiftDirection();
    }

    bool PuzzleObject::isDropSwipe(SwipeDirection swipeDir) {
        switch (cur_focus) {
            case 0: 
                return swipeDir == SWIPE_DOWN;
            case 1: 
                return swipeDir == SWIPE_RIGHT;
            case 2: 
                return swipeDir == SWIPE_UP;
            case 3: 
                return swipeDir == SWIPE_LEFT;
            default:
                return false;
        }
    }

    void PuzzleObject::handleSingleTap(SDL_Point tapPos) {
    }

    bool PuzzleObject::isSwitchSwipe(SwipeDirection swipeDir) {
        switch (cur_focus) {
            case 0: 
                return swipeDir == SWIPE_UP;
            case 1: 
                return swipeDir == SWIPE_LEFT;
            case 2: 
                return swipeDir == SWIPE_DOWN;
            case 3: 
                return swipeDir == SWIPE_RIGHT;
            default:
                return false;
        }
    }


    void PuzzleObject::moveBlockToPosition(SDL_Point position) {
        const int cellWidth = 32;    
        const int cellHeight = 16;   
        int gridOffsetX = 0;         
        int gridOffsetY = 0;         
        switch(cur_focus) {
            case 0: { 
                int gridWidth = game.grid[0].width();
                gridOffsetX = (1280 / 2) - ((gridWidth * cellWidth) / 2);
                gridOffsetY = 0;
                break;
            }
            case 1: { 
                int gridWidth = game.grid[1].width();
                gridOffsetX = 32;
                gridOffsetY = (720 / 2) - ((gridWidth * cellHeight) / 2); 
                break;
            }
            case 2: { 
                int gridWidth = game.grid[2].width();
                gridOffsetX = (1280 / 2) - ((gridWidth * cellWidth) / 2);
                gridOffsetY = (720 / 2) + 5;
                break;
            }
            case 3: { 
                int gridWidth = game.grid[3].width();
                gridOffsetX = 1280 - (gridWidth * cellWidth) - 32;
                gridOffsetY = (720 / 2) - ((gridWidth * cellHeight) / 2); 
                break;
            }
            default:
                return; 
        }

        int gridX = 0;
        int gridY = 0;
        switch(cur_focus) {
            case 0: { 
                gridX = (position.x - gridOffsetX) / cellWidth;
                gridY = (position.y - gridOffsetY) / cellHeight;
                break;
            }
            case 1: { 
                gridX = (position.y - gridOffsetY) / cellHeight;
                gridY = (position.x - gridOffsetX) / cellWidth;
                gridY = game.grid[1].width() - 1 - gridY;
                break;
            }
            case 2: { 
                gridX = (position.x - gridOffsetX) / cellWidth;
                gridY = (position.y - gridOffsetY) / cellHeight;
                gridY = game.grid[2].height() - 1 - gridY;
                break;
            }
            case 3: { 
                gridX = (position.y - gridOffsetY) / cellHeight;
                gridY = (position.x - gridOffsetX) / cellWidth;
                gridY = game.grid[3].width() - 1 - gridY;
                break;
            }
        }
        gridX = std::max(0, std::min(gridX, game.grid[cur_focus].width() - 1));
        gridY = std::max(0, std::min(gridY, game.grid[cur_focus].height() - 1));
        if(cur_focus == 1 || cur_focus == 3 )
            game.grid[cur_focus].game_piece.setPosition(game.grid[1].width()-gridX-1, gridY);
        else
            game.grid[cur_focus].game_piece.setPosition(gridX, gridY);

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
        pause_menu = util::loadTexture(renderer, "start.png");
        paused_large = util::loadFont("font.ttf", 44);
        paused_small = util::loadFont("font.ttf", 20);
      
    }
}