//
// Created by CIJhn on 11/13/2016.
//

#include "syntax_exception.h"

namespace mis {
    syntax_exception::syntax_exception(const std::string &error) : error(error) {}

    const std::string &syntax_exception::getError() const {
        return error;
    }
}