//
// Created by CIJhn on 11/13/2016.
//

#include "mis-core/mis_exception.h"

namespace mis {
    mis_exception::mis_exception(const std::string &error) : error(error) {}

    const std::string &mis_exception::getError() const {
        return error;
    }
}