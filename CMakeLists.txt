cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 11)

project(opinion-game)

find_package(SDL2 REQUIRED)

add_executable(game main.c dynamics.c game.c objectives.c sdl_utils.c state_list.c trajectories.c)

target_link_libraries(game ${SDL2_LIBRARIES})
