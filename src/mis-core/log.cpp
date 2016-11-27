//
// Created by CIJhn on 11/26/2016.
//

#include <string>
#include <iostream>
#include "mis-core/log.h"

namespace mis {
    void _lgcapture(const std::string &&s, const std::string &clz, const std::string &func, int line) {
        std::cout
                << "[" + clz.substr(clz.find_last_of("/") + 1) + ":" + std::to_string(line) + ", " + func + "()" +
                   "] " + s << std::endl;
    }
}