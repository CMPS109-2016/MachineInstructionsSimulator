//
// Created by CIJhn on 11/21/2016.
//

#include "mis-core/restriction.h"
#include "mis-instructions/instructions.h"

namespace mis {
    template<typename T>
    class WorkDclVar : virtual public VWork {
    protected:
        std::string name;
        T defaultValue;
    public:
        WorkDclVar(const std::string &name, T defaultValue) : name(name), defaultValue(defaultValue) {}

        virtual void performance(mis::VirtualMachine::Runtime &runtime, Flow flow) override =0;
    };

    class WorkDclVarNumber : virtual public WorkDclVar<Number> {
    public:
        WorkDclVarNumber(std::string name, Number defaultValue) :
                WorkDclVar(std::move(name), defaultValue) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) {
            runtime.allocate(name, defaultValue);
        };
    };

    class WorkDclVarChar : virtual public WorkDclVar<Char> {
    public:
        WorkDclVarChar(std::string name, Char charSequence) :
                WorkDclVar(name, defaultValue) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) {
            runtime.allocate(name, defaultValue);
        };
    };

    class WorkDclVarString : virtual public WorkDclVar<String> {
    public:
        WorkDclVarString(const std::string &name, const String &defaultValue) : WorkDclVar(name, defaultValue) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) {
            runtime.allocate(name, defaultValue);
        };
    };


    void regDeclVar(Parser::Builder &builder) {
        builder.registerInstructionBuilder("VAR", constrain(
                [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                    VirtualMachine::Work *pt = nullptr;
                    std::string name(tokens[0]->asString());

                    if (context.find(name) != context.end()) {
                        throw mis_exception(
                                "duplicated variable declared! [" + name + "]");
                    }
                    std::string tp(tokens[1]->asString());

                    context[name] = tp;//wtf?

//                    std::cout << context[name] << std::endl;
                    if (tp == _NUMERIC)
                        pt = new WorkDclVarNumber(name,
                                                  tokens[2]->getData().numeric);
                    else if (tp == _REAL)
                        pt = new WorkDclVarNumber(name, tokens[2]->getData().real);
                    else if (tp == _CHAR)
                        pt = new WorkDclVarChar(name,
                                                tokens[2]->getData().character);
                    else if (tp == _STRING)
                        pt = new WorkDclVarString(name, tokens[2]->asString());
                    return pt;
                }, TypeMatcher().after(TokenType::VAR_NAME).after(TokenType::TYPE).
                        after(RangedType() || TokenType::NUMERIC || TokenType::REAL || TokenType::STRING ||
                              TokenType::CHAR)));
    }
}
