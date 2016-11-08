cmake_minimum_required(VERSION 3.6)
project(MachineInstructionsSimulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories(includes)

set(SOURCE_FILES src/mis/main.cpp src/mis/VirtualMachine.cpp includes/mis/VirtualMachine.h includes/mis/strutil.h src/mis/strutil.cpp includes/mis/lang.h src/mis/lang.cpp includes/mis/Parser.h src/mis/Parser.cpp includes/mis/Executor.h src/mis/instructions.cpp includes/mis/UnitBuilderRestriction.cpp includes/mis/UnitBuilderRestriction.h src/mis/Executor.cpp)
add_executable(MachineInstructionsSimulator ${SOURCE_FILES})