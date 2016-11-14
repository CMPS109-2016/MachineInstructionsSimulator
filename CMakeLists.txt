cmake_minimum_required(VERSION 3.6)
project(MachineInstructionsSimulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories(includes)

set(SOURCE_FILES src/mis/main.cpp
        includes/mis/restriction.h src/mis/restriction.cpp
        src/mis/VirtualMachine.cpp includes/mis/VirtualMachine.h
        includes/mis/strutil.h src/mis/strutil.cpp
        includes/mis/lang.h src/mis/lang.cpp
        includes/mis/Parser.h src/mis/Parser.cpp
        includes/mis/Executor.h src/mis/Executor.cpp
        includes/mis/default_instructions.h src/mis/instructions.cpp
        includes/mis/syntax_exception.cpp includes/mis/syntax_exception.h)

#set(SOURCE_FILES src/mis/main.cpp
#        src/mis/restriction.cpp
#        src/mis/VirtualMachine.cpp
#        src/mis/strutil.cpp
#        src/mis/lang.cpp
#        src/mis/Parser.cpp
#        src/mis/Executor.cpp
#        src/mis/instructions.cpp
#        )
add_executable(MachineInstructionsSimulator ${SOURCE_FILES})