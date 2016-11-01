cmake_minimum_required(VERSION 3.6)
project(MachineInstructionsSimulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories(includes)

set(SOURCE_FILES src/mis/main.cpp src/mis/VirtualMachine.cpp includes/mis/VirtualMachine.h includes/mis/strutil.h includes/mis/Instruction.h src/mis/strutil.cpp includes/mis/lang.h src/mis/lang.cpp includes/mis/Parser.h src/mis/Parser.cpp includes/mis/AsyncExecutor.h)
add_executable(MachineInstructionsSimulator ${SOURCE_FILES})