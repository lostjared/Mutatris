cmake_minimum_required(VERSION 3.10)
project(Mutatris LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)

option(WITH_SDL_MIXER "Build with SDL Sound support" ON)

if(WITH_SDL_MIXER)
find_package(SDL2_mixer)
message(STATUS "Mixer enabled.")
add_compile_definitions(HAS_SOUND)
else()
message(STATUS, "Sound disabled")
endif()

include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCES ${CMAKE_SOURCE_DIR}/main.cpp ${CMAKE_SOURCE_DIR}/quadtris.cpp ${CMAKE_SOURCE_DIR}/util.cpp ${CMAKE_SOURCE_DIR}/loadpng.cpp ${CMAKE_SOURCE_DIR}/intro.cpp ${CMAKE_SOURCE_DIR}/puzzle.cpp ${CMAKE_SOURCE_DIR}/game_over.cpp ${CMAKE_SOURCE_DIR}/sound.cpp ${CMAKE_SOURCE_DIR}/startup.cpp)
add_executable(Mutatris ${SOURCES})
if(WITH_SDL_MIXER)
target_link_libraries(Mutatris ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer ${PNG_LIBRARIES} ${ZLIB_LIBRARIES})
else()
target_link_libraries(Mutatris ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf ${PNG_LIBRARIES} ${ZLIB_LIBRARIES})
endif()
target_include_directories(Mutatris PUBLIC ${SDL2_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} /usr/local/include/SDL2 /usr/include/SDL2)
list(APPEND COMPILER_FLAGS "-Wall" "-pedantic" "-O2")
target_compile_options(Mutatris PRIVATE ${COMPILER_FLAGS})
message(STATUS "Got it Configured")