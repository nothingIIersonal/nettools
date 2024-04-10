cmake_minimum_required(VERSION 3.5)
project(examples LANGUAGES C)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 17)

include_directories(
    ../include
    ../xsocket/include
)

add_subdirectory(.. nettools/build)
add_subdirectory(get-ext-ipv4-by-stun)
add_subdirectory(get-ip-by-domain-name)
