cmake_minimum_required(VERSION 3.17)
project(Glad)

add_library(glad STATIC include/glad/glad.h src/glad.c)

target_compile_options(glad PRIVATE -fPIC)

target_include_directories(glad PUBLIC include/)