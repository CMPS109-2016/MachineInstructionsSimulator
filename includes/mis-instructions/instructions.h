//
// Created by CIJhn on 11/4/2016.
//


#ifndef MACHINEINSTRUCTIONSSIMULATOR_DEF_INSTRUCT_H
#define MACHINEINSTRUCTIONSSIMULATOR_DEF_INSTRUCT_H

#include <iostream>
#include <algorithm>
#include <mis-core/mis_exception.h>

#include "mis-core/restriction.h"
#include "mis-instructions/default_instructions.h"
#include "mis-core/strutil.h"

namespace mis {
    const std::string _NUMERIC = "NUMERIC", _REAL = "REAL", _STRING = "STRING", _CHAR = "CHAR";
    using VWork = VirtualMachine::Work;

    template<typename T>
    struct Getter {
        T get(VirtualMachine::Runtime &runtime) {
            return func(runtime, tk);
        }

        Getter(const Parser::Token &tk, const std::function<T(VirtualMachine::Runtime &, Parser::Token &)> &func) :
                tk(tk), func(func) {}

        Getter(const Getter &o) : tk(o.tk), func(o.func) {}

    private:
        Parser::Token tk;
        std::function<T(VirtualMachine::Runtime &, Parser::Token &tk)> func;
    };


    Getter<Number> wrapRef(Parser::Token &token);

    Getter<Number> wrap(Parser::Token *token);

    Getter<std::string> warpToStringRef(Parser::Token &token);

    Getter<std::string> warpToString(Parser::Token *token);

    class WorkPlaceHolder : virtual public VWork {
    public:
        virtual void performance(VirtualMachine::Runtime &runtime, Flow itr) override {}
    };

    void regFilter(Parser::Builder &builder);

    void regDeclVar(Parser::Builder &builder);

    void regMath(Parser::Builder &builder);

    void regAssign(Parser::Builder &builder);

    void regString(Parser::Builder &builder);

    void regJump(Parser::Builder &builder);

    void regMultiThread(Parser::Builder &builder);

    void regOut(Parser::Builder &builder);
}


#endif