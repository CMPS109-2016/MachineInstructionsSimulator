//
// Created by CIJhn on 10/23/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_VARAIABLE_H
#define MACHINEINSTRUCTIONSSIMULATOR_VARAIABLE_H

namespace mis {
    class TokenInst;

    class TokenVariable;

    class TokenVarDecl;

    class TokenLabel;

    class TokenVarTypeDecl;

    class Token {
    public:
        Token();

        ~Token();

        virtual bool isVariable();

        virtual bool isLabel();

        virtual bool isTypeDecl();

        virtual bool isInst();

        TokenVariable &asVariable();

        virtual TokenLabel &asLabel();

        virtual TokenVarTypeDecl &asTypeDecl();

        virtual TokenInst &asInst();

        friend class Tokenizer;
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_VARAIABLE_H
