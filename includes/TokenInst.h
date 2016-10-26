//
// Created by CIJhn on 10/24/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_TOKENINST_H
#define MACHINEINSTRUCTIONSSIMULATOR_TOKENINST_H

#include "Token.h"
#include "Instruction.h"

namespace mis {
    class TokenInst : public Token {
    private:
        Instruction& instruction;
    public:
        virtual bool isInst() override;

        virtual TokenInst &asInst() override;

        Instruction &getInstruction();

        TokenInst(Instruction &inst);
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_TOKENINST_H
