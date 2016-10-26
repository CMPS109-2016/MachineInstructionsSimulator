//
// Created by CIJhn on 10/24/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_COMMAND_H
#define MACHINEINSTRUCTIONSSIMULATOR_COMMAND_H

#include <vector>
#include "Token.h"

namespace mis {
    class Command {

    public:
        TokenInst head();

        std::vector<Token> remain();
    };
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_COMMAND_H
