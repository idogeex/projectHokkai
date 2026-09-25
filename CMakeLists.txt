cmake_minimum_required(VERSION 3.20)

project(ProjectHokkai)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SDL)

add_executable(ProjectHokkai
    src/main.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Projectile.cpp
)

target_link_libraries(ProjectHokkai PRIVATE SDL3::SDL3)