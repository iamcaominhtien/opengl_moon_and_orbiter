cmake_minimum_required(VERSION 3.23)
project(moon_and_orbiter_)

set(CMAKE_CXX_STANDARD 14)

add_executable(moon_and_orbiter_ main.cpp)

target_link_libraries(moon_and_orbiter_ -lOpenGL32 -lfreeGLUT -lGLU32)
