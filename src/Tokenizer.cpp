//
// Created by CIJhn on 10/24/2016.
//

#include "../includes/Tokenizer.h"
#include "strutil.hpp"

std::vector<mis::Token> &mis::Tokenizer::token(const std::string line) {
    std::vector<mis::Token>* vec = new std::vector<mis::Token>;
    std::vector<std::string> str;
    split(line, " ", str);
    if (str.size() != 2)
        throw new std::bad_exception;
    return *vec;
}

mis::Tokenizer::Tokenizer() {
}
