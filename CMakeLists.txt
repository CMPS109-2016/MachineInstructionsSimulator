cmake_minimum_required(VERSION 3.6)
project(MachineInstructionsSimulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES src/main.cpp src/Scope.cpp includes/Scope.h includes/Executor.h src/Token.cpp includes/Token.h src/RestrictedExecutor.cpp includes/RestrictedExecutor.h src/PredicateParameterSize.cpp includes/PredicateParameterSize.h src/PredicateVarAdapt.cpp includes/PredicateVarAdapt.h src/VirtualMachine.cpp includes/VirtualMachine.h src/Executor.cpp includes/TokenVariable.h includes/TokenLabel.h src/Tokenizer.cpp includes/Tokenizer.h src/Command.cpp includes/Command.h includes/TokenInst.h includes/TokenVarTypeDecl.h includes/tokens includes/TokenVarDecl.h includes/TokenVarDecl.h src/strutil.hpp src/Instruction.cpp includes/types includes/Instruction.h)
add_executable(MachineInstructionsSimulator ${SOURCE_FILES})