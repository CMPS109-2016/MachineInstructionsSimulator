#include <iostream>
#include <functional>
#include "mis/strutil.h"
#include "mis/Parser.h"


struct WorkAdd : public mis::VirtualMachine::Work {
    std::string &targetName;
    std::vector<mis::Number> numbers;

    virtual void performance(mis::VirtualMachine::Runtime &runtime) {
        runtime.getNumber(targetName)=sum;
    }
};

mis::VirtualMachine::Work *any(std::vector<mis::Parser::Token> &args) {
    mis::Parser::Token first = args[0];
    if (first.getType() == mis::Parser::Token::Type::VAR) {

    } else {
        throw std::bad_exception;
    }
}

int main() {
    mis::Parser::Builder builder;
    builder.registerInstructionBuilder("ADD", any);

    return 0;
}
