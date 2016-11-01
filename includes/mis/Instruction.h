//
// Created by CIJhn on 10/25/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_INSTRUCTION_H
#define MACHINEINSTRUCTIONSSIMULATOR_INSTRUCTION_H

#include <string>
#include "Parser.h"

namespace mis {
    enum class Instruction {
        ADD, SUB, MUL, DIV, ASSIGN, OUT, SET_STR_CHAR, GET_STR_CHAR, LABEL, JMP, JMPZ, JMPNZ
    };

    static void registerInst(Parser::Builder &parser) {
        parser.registerBuilder("ADD", [](std::vector<Parser::Token> &args) {
            VirtualMachine::Work *p = nullptr;

            return p;
        });
    }
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_INSTRUCTION_H
