//
// Created by CIJhn on 11/21/2016.
//

#include "mis-instructions/instructions.h"

namespace mis {

    class WorkPreLabel : virtual public WorkPlaceHolder {
        std::string label;
    public:
        WorkPreLabel(const std::string &label) : label(label) {}

        const std::string &getLabel() const {
            return label;
        }
    };

    class WorkPreJMP : public WorkPlaceHolder {
        std::string target;
    public:
        WorkPreJMP(const std::string &target) : target(target) {}

        const std::string &getTarget() const {
            return target;
        }
    };

    class WorkPreJMPZ : public WorkPlaceHolder {
        bool isNZ;
        std::string label;
        Getter<Number> getter;

    public:
        WorkPreJMPZ(bool isNZ, const std::string &label, const Getter<mis::Number> &getter) : isNZ(isNZ), label(label),
                                                                                              getter(getter) {}

        bool isIsNZ() const {
            return isNZ;
        }

        const std::string &getLabel() const {
            return label;
        }

        const Getter<mis::Number> &getGetter() const {
            return getter;
        }
    };

    class WorkPreJMPCOMP : public WorkPlaceHolder {
    public:
        enum class Type {
            GT, LT, GTE, LTE
        };

        WorkPreJMPCOMP(Type tp, const Getter<mis::Number> &a, const Getter<mis::Number> &b, const std::string &s) : tp(
                tp), a(a), b(b), s(s) {}

        Type getTp() const {
            return tp;
        }

        const Getter<mis::Number> &getA() const {
            return a;
        }

        const Getter<mis::Number> &getB() const {
            return b;
        }

        const std::string &getLabel() const {
            return s;
        }

    private :
        Type tp;
        Getter<Number> a, b;
        std::string s;
    };

    class WorkJump : public VWork {
        std::vector<VirtualMachine::Work *>::iterator itr;
    public:
        WorkJump(const std::vector<mis::VirtualMachine::Work *>::iterator &itr)
                : itr(itr) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            flow = itr;
        }
    };

    class WorkJumpZ : public WorkJump {
        Getter<Number> getter;
        bool nz;
    public:
        WorkJumpZ(const std::vector<mis::VirtualMachine::Work *>::iterator &itr, const Getter<mis::Number> &getter,
                  bool nz) : WorkJump(itr), getter(getter), nz(nz) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            mis::log("performance jumpz");
            bool isZ = getter.get(runtime) == 0;
            if (nz && !isZ)
                WorkJump::performance(runtime, flow);
            else if (isZ)
                WorkJump::performance(runtime, flow);
        }
    };

    class WorkJumpCompare : public WorkJump {
        Getter<Number> a, b;
        WorkPreJMPCOMP::Type T;
    public:
        WorkJumpCompare(const std::vector<mis::VirtualMachine::Work *>::iterator &itr, const Getter<mis::Number> &a,
                        const Getter<mis::Number> &b, WorkPreJMPCOMP::Type T) : WorkJump(itr), a(a), b(b), T(T) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            switch (T) {
                case WorkPreJMPCOMP::Type::GT:
                    if (a.get(runtime) > b.get(runtime))
                        WorkJump::performance(runtime, flow);
                    break;
                case WorkPreJMPCOMP::Type::LT:
                    if (a.get(runtime) < b.get(runtime))
                        WorkJump::performance(runtime, flow);
                    break;
                case WorkPreJMPCOMP::Type::GTE:
                    if (a.get(runtime) >= b.get(runtime))
                        WorkJump::performance(runtime, flow);
                    break;
                case WorkPreJMPCOMP::Type::LTE:
                    if (a.get(runtime) <= b.get(runtime))
                        WorkJump::performance(runtime, flow);
                    break;
            }
        }
    };


}

void mis::regJump(Parser::Builder &builder) {
    mis::log("register jump");
    RangedType numberOrVar = RangedType()
                             || TokenType::NUMERIC || TokenType::REAL
                             || TokenType::VAR_REAL || TokenType::VAR_NUMERIC;

    builder.registerInstructionBuilder("LABEL",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreLabel(std::string(tokens[0]->asString()));
                                               }, TypeMatcher().after(TokenType::PLAIN_TEXT)));

    builder.registerInstructionBuilder("JMP",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreJMP(std::string(tokens[0]->asString()));
                                               }, TypeMatcher().after(TokenType::PLAIN_TEXT)));

    auto jumpzPred = TypeMatcher().after(TokenType::PLAIN_TEXT).after(numberOrVar);
    builder.registerInstructionBuilder("JMPZ",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreJMPZ(false, std::string(tokens[0]->asString()),
                                                                          wrap(tokens[1]));
                                               }, jumpzPred));

    builder.registerInstructionBuilder("JMPNZ",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreJMPZ(true, std::string(tokens[0]->asString()),
                                                                          wrap(tokens[1]));
                                               }, jumpzPred));

    auto jumpcpPred = TypeMatcher().after(TokenType::PLAIN_TEXT).after(numberOrVar).after(numberOrVar);
    builder.registerInstructionBuilder("JMPGT",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreJMPCOMP(WorkPreJMPCOMP::Type::GT,
                                                                             wrap(tokens[1]),
                                                                             wrap(tokens[2]),
                                                                             std::string(tokens[0]->asString()));
                                               }, jumpcpPred));

    builder.registerInstructionBuilder("JMPLT",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreJMPCOMP(WorkPreJMPCOMP::Type::LT,
                                                                             wrap(tokens[1]),
                                                                             wrap(tokens[2]),
                                                                             std::string(tokens[0]->asString()));
                                               }, jumpcpPred));

    builder.registerInstructionBuilder("JMPGTE",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreJMPCOMP(WorkPreJMPCOMP::Type::GTE,
                                                                             wrap(tokens[1]),
                                                                             wrap(tokens[2]),
                                                                             std::string(tokens[0]->asString()));
                                               }, jumpcpPred));
    builder.registerInstructionBuilder("JMPLTE",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkPreJMPCOMP(WorkPreJMPCOMP::Type::LTE,
                                                                             wrap(tokens[1]),
                                                                             wrap(tokens[2]),
                                                                             std::string(tokens[0]->asString()));
                                               }, jumpcpPred));
    builder.registerLinker([](std::vector<VirtualMachine::Work *> &works) {
        std::map<std::string, std::vector<VirtualMachine::Work *>::iterator> labelMap;
        for (auto itr = works.begin(); itr != works.end(); ++itr) {
            WorkPreLabel *label = dynamic_cast<WorkPreLabel *>(*itr);
            if (label != nullptr)
                labelMap[label->getLabel()] = itr;
        }
        for (auto itr = works.begin(); itr != works.end(); ++itr) {
            VirtualMachine::Work *work = *itr;
            if (instanceof<WorkPreJMP, VWork>(work)) {
                WorkPreJMP *jp = dynamic_cast<WorkPreJMP *>(work);
                auto target = labelMap.find(jp->getTarget());
                if (target != labelMap.end()) {
                    *itr = new WorkJump((*target).second);
                } else {
                    //not found
                    throw mis_exception("no such label found named [" + jp->getTarget() + "]");
                }
            } else if (instanceof<WorkPreJMPCOMP, VWork>(work)) {
                WorkPreJMPCOMP *jp = dynamic_cast<WorkPreJMPCOMP *>(work);
                auto target = labelMap.find(jp->getLabel());
                if (target != labelMap.end()) {
                    *itr = new WorkJumpCompare((*target).second, jp->getA(), jp->getB(), jp->getTp());
                } else {
                    //not found
                    throw mis_exception("no such label found named [" + jp->getLabel() + "]");
                }
            } else if (instanceof<WorkPreJMPZ, VWork>(work)) {
                WorkPreJMPZ *jp = dynamic_cast<WorkPreJMPZ *>(work);
                auto target = labelMap.find(jp->getLabel());
                if (target != labelMap.end()) {
                    *itr = new WorkJumpZ((*target).second, jp->getGetter(), jp->isIsNZ());
                } else {
                    //not found
                    throw mis_exception("no such label found named [" + jp->getLabel() + "]");
                }
            }
        }
    });

}

