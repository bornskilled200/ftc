cmake_minimum_required(VERSION 2.8.4)
project(ftc C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES main.c)
add_executable(ftc ${SOURCE_FILES})

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc")
if (WIN32)
    target_link_libraries(ftc libuv.a libws2_32.a libpsapi.a libiphlpapi.a)
endif(WIN32)