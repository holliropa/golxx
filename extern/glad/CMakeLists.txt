cmake_minimum_required(VERSION 3.27)
project(glad VERSION 2.0.0 DESCRIPTION "GLAD simplifies the process of managing OpenGL function pointers.")

include_directories(include)
add_library(glad src/gl.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)