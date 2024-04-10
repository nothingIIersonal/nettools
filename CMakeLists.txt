cmake_minimum_required(VERSION 3.5)
project(nettools LANGUAGES C)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 17)

add_subdirectory(xsocket)

add_library(${PROJECT_NAME} STATIC
    src/nettools/nettools.c
)
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    xsocket/include
)
target_link_libraries(${PROJECT_NAME}
    xsocket
)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
)
