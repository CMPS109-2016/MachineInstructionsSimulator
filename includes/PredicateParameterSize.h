//
// Created by CIJhn on 10/23/2016.
//


#ifndef MACHINEINSTRUCTIONSSIMULATOR_PARAMETERSIZERESTRICTION_H
#define MACHINEINSTRUCTIONSSIMULATOR_PARAMETERSIZERESTRICTION_H

#include <stdexcept>
#include "RestrictedExecutor.h"

namespace mis {
    class PredicateParameterSize : Predicate {
    private:
        const int min, max;
    public:
        virtual void validate(std::vector<Token> &vars)throw(std::invalid_argument) override;

        PredicateParameterSize(int min, int max);

        ~PredicateParameterSize();
    };

}
#endif //MACHINEINSTRUCTIONSSIMULATOR_PARAMETERSIZERESTRICTION_H
