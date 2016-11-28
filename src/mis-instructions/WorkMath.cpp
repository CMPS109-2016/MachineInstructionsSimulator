//
// Created by CIJhn on 11/21/2016.
//

#include "mis-instructions/instructions.h"

namespace mis {

    class WorkVarArgs : virtual public VWork {
    protected:
        std::string target;
        std::vector<Getter<Number>> src;

    public:
        WorkVarArgs(std::string target, const std::vector<Getter<Number>> &src) : target(std::move(target)),
                                                                                  src(std::move(src)) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override =0;
    };

    class WorkAdd : public WorkVarArgs {
    public:
        WorkAdd(std::string target, const std::vector<Getter<Number>> &src) : WorkVarArgs(std::move(target),
                                                                                          std::move(src)) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            Number *n = runtime.getNumber(target);
            if (n != nullptr) {
                Number &ref = *n;
                for (Getter<Number> &get : src)
                    ref += get.get(runtime, <#initializer#>);
            } else {
                runtime.halt("Runtime error. No such variable named" + target + ".", flow);
            }
        }
    };

    class WorkSub : public VWork {
        std::string target;
        Getter<Number> a, b;

    public:
        WorkSub(std::string target, Getter<Number> a, Getter<Number> b) : target(std::move(target)),
                                                                          a(std::move(a)),
                                                                          b(std::move(b)) {}

        virtual ~WorkSub() {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            Number *np = runtime.getNumber(target);
            if (np != nullptr)
                (*np) = a.get(runtime, <#initializer#>) - b.get(runtime, <#initializer#>);
        }
    };

    class WorkDiv : public VWork {
        std::string target;
        Getter<Number> a, b;

    public:
        WorkDiv(std::string target, Getter<Number> a, Getter<Number> b) :
                target(std::move(target)), a(std::move(a)), b(std::move(b)) {}

        virtual ~WorkDiv() {
        }

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            Number *np = runtime.getNumber(target);
            if (np != nullptr) {
                Number &&an = b.get(runtime, <#initializer#>);
                Number &&bn = a.get(runtime, <#initializer#>);
                if (bn == 0) {
                } else
                    (*np) = an / bn;
            }
        }
    };

    class WorkMul : public WorkVarArgs {
    public:
        WorkMul(std::string target, const std::vector<Getter<Number>> src) : WorkVarArgs(std::move(target),
                                                                                         std::move(src)) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            Number *n = runtime.getNumber(target);
            if (n != nullptr) {
                Number &ref = *n;
                try {
                    for (Getter<Number> &get : src)
                        ref *= get.get(runtime, flow);
                } catch (mis_exception e) {
                    runtime.halt(e.getError(), flow);
                }
            } else {
                runtime.halt("Runtime error. No such variable named" + target + ".", flow);
            }
        }
    };

    void regMath(Parser::Builder &builder) {
        RangedType numberOrVar = RangedType()
                                 || TokenType::NUMERIC || TokenType::REAL
                                 || TokenType::VAR_REAL || TokenType::VAR_NUMERIC,
                numberVar = RangedType() || TokenType::VAR_NUMERIC || TokenType::VAR_REAL;

        auto
                addMul = TypeMatcher().after(numberVar).after(
                VariantArguments<RangedType>(numberOrVar).setMax(12).setMin(2)),
                subDiv = TypeMatcher().after(numberVar)
                .after(numberOrVar)
                .after(numberOrVar);
        builder.registerInstructionBuilder("ADD",
                                           constrain(
                                                   [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                       VirtualMachine::Work *pt = nullptr;
                                                       std::string varName(tokens[0]->asString());
                                                       std::vector<mis::Getter<Number>> src;
                                                       for (int i = 1; i < tokens.size(); ++i)
                                                           src.push_back(wrap(tokens[i]));
                                                       pt = new WorkAdd(varName, src);
                                                       return pt;
                                                   }, addMul));

        builder.registerInstructionBuilder("SUB",
                                           constrain(
                                                   [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                       return new WorkSub(std::string(tokens[0]->asString()),
                                                                          wrap(tokens[1]), wrap(tokens[2]));
                                                   }, subDiv));

        builder.registerInstructionBuilder("MUL",
                                           constrain(
                                                   [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                       VirtualMachine::Work *pt = nullptr;
                                                       std::vector<mis::Getter<Number>> src;
                                                       for (int i = 1; i < tokens.size(); ++i)
                                                           src.push_back(wrap(tokens[i]));
                                                       pt = new WorkMul(std::string(tokens[0]->asString()), src);
                                                       return pt;
                                                   }, addMul));


        builder.registerInstructionBuilder("DIV",
                                           constrain(
                                                   [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                       return new WorkDiv(std::string(tokens[0]->asString()),
                                                                          wrap(tokens[1]), wrap(tokens[2]));
                                                   }, subDiv));
    }

}
