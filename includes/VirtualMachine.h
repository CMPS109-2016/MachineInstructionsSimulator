//
// Created by CIJhn on 10/23/2016.
//


#ifndef MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
#define MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H

#include "Scope.h"
#include "Command.h"
#include <vector>

namespace mis {

    class VirtualMachine {
    public:
        Scope &execute(Command &command);

        Scope &execute(Scope &scope, Command &command);

        Scope &execute(std::vector<Command &> commands);

        Scope &execute(Scope &scope, std::vector<Command &> commands);

        bool isTerminated();

        void terminate();
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
