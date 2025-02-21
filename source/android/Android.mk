LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
APP_ALLOW_MISSING_DEPS := true

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_CPPFLAGS := -std=c++17 -DHAS_SOUND
APP_STL := c++_static

# Add your application source files here...
LOCAL_SRC_FILES := game_over.cpp intro.cpp loadpng.cpp puzzle.cpp quadtris.cpp sound.cpp startup.cpp util.cpp main.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_image SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid -lc++_static -lc++abi

include $(BUILD_SHARED_LIBRARY)
