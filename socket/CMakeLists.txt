cmake_minimum_required(VERSION 2.8.7)
project(socket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

include_directories(include)

set(SOURCE_FILES
        include/common.h
        include/includes.h
        include/TCPSocket.h
        include/TCPServerSocket.h
        TCPSocket.cpp
        TCPServerSocket.cpp
        )

add_executable(socket ${SOURCE_FILES})
