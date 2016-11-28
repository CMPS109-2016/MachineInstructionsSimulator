//
// Created by CIJhn on 11/21/2016.
//

#include "mis-instructions/instructions.h"

namespace mis {
    template<typename T>
    class WorkAssign : public VWork {
        std::string to;
        std::string from;
    public:
        WorkAssign(std::string to, std::string from) : to(std::move(to)), from(std::move(from)) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            T *a = get(to, runtime), *b = get(from, runtime);
            if (a == nullptr || b == nullptr) {
                throw mis_exception("Runtime error on assign!");
            }
            *get(to, runtime) = *get(from, runtime);
        }

        virtual T *get(std::string &name, VirtualMachine::Runtime &runtime)=0;
    };

    class WorkAssignNumber : public WorkAssign<Number> {
    public:
        WorkAssignNumber(const std::string &to, const std::string &from) : WorkAssign(to, from) {}

        virtual Number *get(std::string &name, VirtualMachine::Runtime &runtime) override {
            return runtime.getNumber(name);
        }
    };

    class WorkAssignString : public WorkAssign<String> {
    public:
        WorkAssignString(const std::string &to, const std::string &from) : WorkAssign(to, from) {}

        virtual String *get(std::string &name, VirtualMachine::Runtime &runtime) override {
            CharSequence *pt = runtime.getChars(name);
            if (pt == nullptr) return nullptr;
            if (dynamic_cast<String *> (pt))
                return dynamic_cast<String * > (pt);
        }
    };

    class WorkAssignChar : public WorkAssign<Char> {
    public:
        WorkAssignChar(const std::string &to, const std::string &from) : WorkAssign(to, from) {}

        virtual Char *get(std::string &name, VirtualMachine::Runtime &runtime) override {
            CharSequence *pt = runtime.getChars(name);
            if (pt == nullptr) return nullptr;
            if (dynamic_cast<Char *> (pt))
                return dynamic_cast<Char * > (pt);
        }
    };
}

void mis::regAssign(Parser::Builder &builder) {
    RangedType allVar = RangedType() ||
                        TokenType::VAR_NUMERIC || TokenType::VAR_REAL || TokenType::VAR_STRING
                        || TokenType::VAR_CHAR;
    builder.registerInstructionBuilder("ASSIGN",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   VirtualMachine::Work *pt = nullptr;

                                                   std::string aName(tokens[0]->asString()), bName(
                                                           tokens[1]->asString());
                                                   auto aVar = context.find(aName), bVar = context.find(bName);
                                                   if (aVar == context.end())
                                                       throw mis_exception("No such var named [" + aName + "]");
                                                   if (bVar == context.end())
                                                       throw mis_exception("No such var named [" + bName + "]");
                                                   if (*aVar != *bVar)
                                                       throw mis_exception(
                                                               "var [" + aName +
                                                               "] and var [" + bName +
                                                               "] are in different type, cannot assign!");
                                                   auto type = (*aVar).second;
                                                   if (type == _NUMERIC || type == _REAL)
                                                       pt = new WorkAssignNumber(std::string(tokens[0]->asString()),
                                                                                 std::string(
                                                                                         tokens[1]->asString()));
                                                   else if (type == _STRING)
                                                       pt = new WorkAssignString(std::string(tokens[0]->asString()),
                                                                                 std::string(
                                                                                         tokens[1]->asString()));
                                                   else
                                                       pt = new WorkAssignChar(std::string(tokens[0]->asString()),
                                                                               std::string(tokens[1]->asString()));
                                                   return pt;
                                               }, TypeMatcher().after(allVar).after(allVar)));
}