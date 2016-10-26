//
// Created by CIJhn on 10/23/2016.
//


#ifndef MACHINEINSTRUCTIONSSIMULATOR_RESTRICEDEXE_H
#define MACHINEINSTRUCTIONSSIMULATOR_RESTRICEDEXE_H

#include "Executor.h"

namespace mis {
    class Predicate {
    public:
        virtual void validate(std::vector<Token> &vars)throw(std::invalid_argument)=0;
    };

    class RestrictedExecutor : public Executor {
    private:
        std::vector<Predicate> &restrictions;
    public:
        virtual void execute(std::vector<Token> &vars)throw(std::invalid_argument) override final;

        virtual void doExecute(std::vector<Token> &vars)=0;

        RestrictedExecutor(std::vector<Predicate> &restrictions);

        ~RestrictedExecutor();
    };
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_RESTRICEDINSTRUCTION_H
