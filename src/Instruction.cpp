//
// Created by CIJhn on 10/25/2016.
//

#include <iostream>
#include "../includes/Instruction.h"

namespace mis {
    struct Instruction {
    private:
        const std::string name;

    public:
        const std::string &getName() const { return name; }

        Instruction(const std::string &name) : name(name) {}
    };

    const std::string &to_string(const Instruction &inst) {
        return inst.getName();
    }

    const Instruction ADD("ADD"), SUB("SUB"), MUL("MUL"), DIV("DIV"), ASSIGN("ASSIGN"),
            OUT("OUT"), SET_STR_CHAR("SET_STR_CHAR"), GET_STR_CHAR("GET_STR_CHAR"),
            LABEL("LABEL"), JMP("JMP"), JMPZ("JMPZ"), JMPNZ("JMPNZ");
}


