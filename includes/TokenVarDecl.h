//
// Created by CIJhn on 10/24/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_TOKENVARDECL_H
#define MACHINEINSTRUCTIONSSIMULATOR_TOKENVARDECL_H

#include "Token.h"
#include <string>

namespace mis {
    class TokenVarDecl : public Token {
    private:
        std::string name;
    public:
        std::string getName();
    };

}


#endif //MACHINEINSTRUCTIONSSIMULATOR_TOKENVARDECL_H
