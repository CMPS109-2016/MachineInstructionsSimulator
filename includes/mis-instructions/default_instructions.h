//
// Created by CIJhn on 11/12/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_DEFAULT_INSTRUCTIONS_H
#define MACHINEINSTRUCTIONSSIMULATOR_DEFAULT_INSTRUCTIONS_H

#include "mis-core/parser.h"

namespace mis {
    /**
     * Build the default parser.
     */
    void register_default(Parser::Builder &builder);
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_DEFAULT_INSTRUCTIONS_H
