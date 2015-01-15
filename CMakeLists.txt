cmake_minimum_required(VERSION 2.8.4)
project(ftc C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")


set(SOURCE_FILES main.c)
add_executable(ftc ${SOURCE_FILES})


find_package(LibUV REQUIRED)
#include_directories(${LIBUV_INCLUDE_DIRS})
target_link_libraries(ftc libuv)