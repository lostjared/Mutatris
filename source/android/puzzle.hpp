#ifndef _PUZZLE_H__
#define _PUZZLE_H__

#include"game_object.hpp"
#include"SDL.h"
#include"SDL_ttf.h"
#include"quadtris.hpp"
#include<vector>
#include<unordered_map>

namespace obj {

    enum SwipeDirection {
        SWIPE_UP,
        SWIPE_DOWN,
        SWIPE_LEFT,
        SWIPE_RIGHT,
        SWIPE_NONE
    };

    class PuzzleObject : public GameObject {
    public:
        PuzzleObject(int diff);
        virtual ~PuzzleObject();
        virtual void draw(SDL_Renderer *renderer) override;
        virtual void event(SDL_Renderer *renderer, SDL_Event &e) override;
        virtual void load(SDL_Renderer *renderer) override;
        void drawGrid(SDL_Renderer *renderer, SDL_Texture *tex, int focus);
    protected:
        puzzle::PuzzleGame game;
        std::vector<SDL_Texture *> blocks;
        std::vector<SDL_Texture *> backgrounds;
        SDL_Texture *pause_menu = nullptr;
        bool paused = false;
        int cur_level = 0;
        int cur_focus = 0;
        SDL_Point swipeStartPos = {0, 0};
        Uint32 swipeStartTime = 0;
        const int SWIPE_DISTANCE_THRESHOLD = 50; 
        const Uint32 SWIPE_TIME_THRESHOLD = 500; 
        bool isTapPending = false;
        Uint32 lastTapTime = 0;       
        SDL_Point lastTapPos = {0, 0}; 
        const Uint32 DOUBLE_TAP_THRESHOLD = 200; 
        const int DOUBLE_TAP_DISTANCE = 50;    
        SDL_Texture *game_textures[4] = {nullptr};
        TTF_Font *font = nullptr;
        TTF_Font *paused_small, *paused_large;
        void twistColors();
        void procHat(int cur_focus, int value);
        void procStick(int cur_focus, int axis_x, int axis_y);
        void procDPad(int cur_focus, int button);
        void moveBlockToPosition(SDL_Point position);
        void detectSwipe(SDL_Point startPos, SDL_Point endPos, Uint32 startTime, Uint32 endTime);
        bool isDropSwipe(SwipeDirection swipeDir);
        bool isSwitchSwipe(SwipeDirection swipeDir);
        void handleDoubleTap(SDL_Point p);
        void handleSingleTap(SDL_Point tapPos);
        void handleControllerButtonRepeat(SDL_GameController *controller, int cur_focus, Uint32 current_time);
        const Uint32 REPEAT_DELAY = 300;   
        const Uint32 REPEAT_INTERVAL = 150; 
        struct ButtonState {
            bool is_pressed;
            Uint32 last_repeat_time;
        };
        std::unordered_map<SDL_GameControllerButton, ButtonState> button_states;
    };

}

#endif