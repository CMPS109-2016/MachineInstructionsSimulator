//
// Created by CIJhn on 10/24/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_TOKENTYPEDECL_H
#define MACHINEINSTRUCTIONSSIMULATOR_TOKENTYPEDECL_H

#include "Token.h"

namespace mis {
    enum class VarType {
        NUMERIC, REAL, CHAR, STRING
    };

    class TokenVarTypeDecl : public Token {
    private:
        VarType type;
    public:
        VarType getDeclType();

        TokenVarTypeDecl(VarType type);
    };
}


#endif //MACHINEINSTRUCTIONSSIMULATOR_TOKENTYPEDECL_H
