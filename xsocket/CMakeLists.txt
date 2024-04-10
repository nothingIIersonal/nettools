cmake_minimum_required(VERSION 3.5)
project(xsocket LANGUAGES C)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 17)

add_library(${PROJECT_NAME} STATIC
    src/xsocket/xsocket.c
)
target_include_directories(${PROJECT_NAME} PRIVATE
    include
)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
)
