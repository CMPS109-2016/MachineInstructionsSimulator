//
// Created by CIJhn on 10/24/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_TOKENVARIABLE_H
#define MACHINEINSTRUCTIONSSIMULATOR_TOKENVARIABLE_H

#include "Token.h"
#include "tokens"

namespace mis {
    class TokenVariable : public Token {
    private:
        VarType type;
        bool isRightValue;
    public:
        TokenVariable(VarType type, auto value, bool isRightValue);

        VarType getType();

        auto getValue();
    };
}


#endif //MACHINEINSTRUCTIONSSIMULATOR_TOKENVARIABLE_H
