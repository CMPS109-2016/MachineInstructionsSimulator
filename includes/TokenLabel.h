//
// Created by CIJhn on 10/24/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_TOKENLABLE_H
#define MACHINEINSTRUCTIONSSIMULATOR_TOKENLABLE_H

#include "Token.h"

namespace mis {
    class TokenLabel : public Token {
    private:
        int lineNumber;
    public:
        TokenLabel(const int lineNumber);

        int getLineNumber();
    };

}
#endif //MACHINEINSTRUCTIONSSIMULATOR_TOKENLABLE_H
