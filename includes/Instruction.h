//
// Created by CIJhn on 10/25/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_INSTRUCTION_H
#define MACHINEINSTRUCTIONSSIMULATOR_INSTRUCTION_H

#include <string>

namespace mis {
    struct Instruction;

    extern const Instruction ADD, SUB, MUL, DIV, ASSIGN, OUT, SET_STR_CHAR, GET_STR_CHAR, LABEL, JMP, JMPZ, JMPNZ;

    const std::string &to_string(const Instruction &inst);
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_INSTRUCTION_H
