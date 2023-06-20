cmake_minimum_required(VERSION 3.5)
project(nettols LANGUAGES C)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 17)

include_directories(include)

add_subdirectory(examples)