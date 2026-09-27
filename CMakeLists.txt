cmake_minimum_required(VERSION 3.20)

project(ProjectHokkai)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

add_subdirectory(external/SDL)

FetchContent_Declare(
    SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG main
)

FetchContent_MakeAvailable(SDL_ttf)

add_executable(ProjectHokkai WIN32
    src/main.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Projectile.cpp
)

target_link_libraries(ProjectHokkai PRIVATE 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf)