cmake_minimum_required(VERSION 2.8.4)
project(ftc C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

set(SOURCE_FILES main.c)

add_executable(ftc ${SOURCE_FILES})
target_link_libraries(ftc uv)

if(WIN32)
    target_link_libraries(ftc ws2_32 psapi iphlpapi Userenv)
endif(WIN32)



