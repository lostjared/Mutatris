CXX=em++
LIBS_PATH=./emscripten-libs
ZLIB_INCLUDE= -I${LIBS_PATH}/zlib/include
PNG_INCLUDE=-I${LIBS_PATH}/libpng/include/libpng16
ZLIB_LIB=$(LIBS_PATH)/zlib/lib/libz.a
PNG_LIB=$(LIBS_PATH)/libpng/lib/libpng16.a
CFLAGS= -std=c++20 -DFOR_WASM -DHAS_SOUND -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 $(PNG_INCLUDE) $(ZLIB_INCLUE)
LDFLAGS= -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 $(PNG_LIB) $(ZLIB_LIB) --preload-file assets
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.cpp=.o)))

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
  
all: $(OBJ_FILES) 
	$(CXX) $(OBJ_FILES)  -o Mutatris.html  $(LDFLAGS)

clean:
	rm -f *.html *.js *.o *.wasm *.data
