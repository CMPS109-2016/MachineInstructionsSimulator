//
// Created by CIJhn on 10/23/2016.
//



#ifndef MACHINEINSTRUCTIONSSIMULATOR_EXE_H
#define MACHINEINSTRUCTIONSSIMULATOR_EXE_H

#include "tokens"
#include <string>
#include <vector>
#include <stdexcept>

namespace mis {
    class Executor {
    public:
        virtual void execute(std::vector<Token> &vars)throw(std::invalid_argument)= 0;
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_INSTRUCTION_H
