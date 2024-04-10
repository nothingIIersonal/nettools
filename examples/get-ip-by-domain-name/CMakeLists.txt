cmake_minimum_required(VERSION 3.5)
project(get-ip-by-domain-name LANGUAGES C)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 17)

add_executable(${PROJECT_NAME}
    main.c
)
target_link_libraries(${PROJECT_NAME}
    nettools
)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
)
