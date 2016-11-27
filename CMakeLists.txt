cmake_minimum_required(VERSION 3.6)
project(MachineInstructionsSimulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories(includes)

set(SOURCE_FILES src/mis-core/main.cpp
        includes/mis-core/restriction.h src/mis-core/restriction.cpp
        src/mis-core/VirtualMachine.cpp includes/mis-core/VirtualMachine.h
        includes/mis-core/strutil.h src/mis-core/strutil.cpp
        includes/mis-core/lang.h src/mis-core/lang.cpp
        includes/mis-core/Parser.h src/mis-core/Parser.cpp
        includes/mis-core/Executor.h src/mis-core/Executor.cpp
        src/mis-core/mis_exception.cpp includes/mis-core/mis_exception.h

        includes/mis-instructions/default_instructions.h
        includes/mis-instructions/instructions.h
        src/mis-instructions/WorkDeclVar.cpp
        src/mis-instructions/WorkMath.cpp
        src/mis-instructions/WorkAssign.cpp
        src/mis-instructions/WorkOut.cpp
        src/mis-instructions/WorkString.cpp
        src/mis-instructions/WorkJump.cpp
        src/mis-instructions/WorkMultiThread.cpp
        src/mis-instructions/WorkOut.cpp
        src/mis-instructions/instruction_impl.cpp src/mis-core/log.cpp includes/mis-core/log.h)

add_executable(MachineInstructionsSimulator ${SOURCE_FILES})
#
#project(mis_socket)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#include_directories(includes/socket)
#
#set(SOURCE_FILES
#        includes/socket/common.h
#        includes/socket/includes.h includes/socket/TCPServerSocket.h
#        includes/socket/TCPSocket.h includes/socket/UDPClientSocket.h
#        includes/socket/UDPServerSocket.h includes/socket/UDPSocket.h
#
#        src/socket/TCPServerSocket.cpp src/socket/TCPSocket.cpp
#        src/socket/UDPClientSocket.cpp src/socket/UDPServerSocket.cpp
#        src/socket/UDPSocket.cpp
#
#        )
#
#add_executable(mis_socket ${SOURCE_FILES})
#
#
