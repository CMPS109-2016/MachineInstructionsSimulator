//
// Created by CIJhn on 10/23/2016.
//

#include <functional>
#include "../includes/tokens"

using namespace mis;

TokenVariable &Token::asVariable() {
    return *dynamic_cast<TokenVariable *>(this);
}

TokenLabel &Token::asLabel() {
    return *dynamic_cast<TokenLabel *>(this);
}

TokenVarTypeDecl &Token::asTypeDecl() {
    return *dynamic_cast<TokenVarTypeDecl *>(this);
}

TokenInst &Token::asInst() {
    throw new std::bad_function_call();
}

bool Token::isVariable() {
    return false;
}

bool Token::isLabel() {
    return false;
}

bool Token::isTypeDecl() {
    return false;
}

bool Token::isInst() {
    return false;
}

Token::Token() {

}

Token::~Token() {

}

bool TokenInst::isInst() {
    return Token::isInst();
}

TokenInst &TokenInst::asInst() {
    return Token::asInst();
}

Instruction &TokenInst::getInstruction() {
    return instruction;
}

TokenLabel::TokenLabel(const int lineNumber) : lineNumber(lineNumber) {
}

int TokenLabel::getLineNumber() {
    return lineNumber;
}


VarType TokenVarTypeDecl::getDeclType() {
    return type;
}

TokenVarTypeDecl::TokenVarTypeDecl(VarType type) : type(type) {
}


std::string TokenVarDecl::getName() {
    return name;
}