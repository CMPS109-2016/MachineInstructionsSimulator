cmake_minimum_required(VERSION 2.8.7)
project(MachineInstructionsSimulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
include_directories(includes)
include_directories(socket/include)
add_subdirectory(socket)

set(SOURCE_FILES src/mis-client/ClientBoost.cpp
        includes/mis-core/restriction.h src/mis-core/restriction.cpp
        src/mis-core/VirtualMachine.cpp includes/mis-core/VirtualMachine.h
        includes/mis-core/strutil.h src/mis-core/strutil.cpp
        includes/mis-core/lang.h src/mis-core/lang.cpp
        includes/mis-core/parser.h src/mis-core/parser.cpp
        src/mis-core/mis_exception.cpp includes/mis-core/mis_exception.h
        src/mis-core/log.cpp includes/mis-core/log.h
        includes/mis-core/bit.h src/mis-core/bit.cpp

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
        src/mis-instructions/instruction_impl.cpp

        src/mis-client/MISClient.cpp includes/mis-client/MISClient.h
        src/mis-server/MISServer.cpp includes/mis-server/MISServer.h
        src/mis-server/ServerBoost.cpp)

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
