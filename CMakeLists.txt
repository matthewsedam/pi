cmake_minimum_required(VERSION 3.6)
project(pi)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.c src/pi.c src/pi.h)
add_executable(pi ${SOURCE_FILES})
target_link_libraries(pi libgmp)
