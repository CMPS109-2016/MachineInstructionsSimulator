//
// Created by CIJhn on 11/4/2016.
//

#include <iostream>
#include <algorithm>
#include <mis/syntax_exception.h>

#include "mis/restriction.h"
#include "mis/default_instructions.h"
#include "mis/strutil.h"

namespace mis {
    const std::string _NUMERIC = "NUMERIC", _REAL = "REAL", _STRING = "STRING", _CHAR = "CHAR";

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

    using VWork = VirtualMachine::Work;


    Getter<Number> wrap(Parser::Token &token) {
        if (token.getType() == Parser::Token::Type::NUMBER)
            return Getter<Number>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                return Number(tk.getData().numeric);
            });
        else if (token.getType() == Parser::Token::Type::REAL)
            return Getter<Number>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                return Number(tk.getData().real);
            });
        else if (token.getType() == Parser::Token::Type::VAR_NUMERIC ||
                 token.getType() == Parser::Token::Type::VAR_REAL)
            return Getter<Number>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                std::string temp(tk.asString());
                std::string &name = temp;
                Number *n = runtime.getNumber(name);
                if (n == nullptr)
                    runtime.report("Not found the var named " + name, true);
                return *n;
            });
        else throw syntax_exception("Token is not a number/real.");
    }

    Getter<Number> wrap(Parser::Token *token) {
        return wrap(*token);
    }

    Getter<std::string> warpToString(Parser::Token &token) {
        if (token.getType() != TokenType::PLAIN_TEXT && token.getType() != TokenType::TYPE &&
            token.getType() != TokenType::VAR_NAME) {
            if (token.getType() == Parser::Token::Type::NUMBER)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    return std::to_string(tk.getData().numeric);
                });
            else if (token.getType() == Parser::Token::Type::REAL)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    return std::to_string(tk.getData().real);
                });
            else if (token.getType() == Parser::Token::Type::VAR_NUMERIC ||
                     token.getType() == Parser::Token::Type::VAR_REAL)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    std::string temp(tk.asString());
                    Number *n = runtime.getNumber(temp);
                    if (n == nullptr) {
                        runtime.report("Not found the var named [" + temp + "]", true);
                        return std::string();
                    }
                    return n->to_string();
                });
            else if (token.getType() == TokenType::STRING)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    return std::string(tk.asString());
                });
            else if (token.getType() == TokenType::CHAR)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    std::string s;
                    s.push_back(tk.getData().character);
                    return s;
                });
            else if (token.getType() == Parser::Token::Type::VAR_CHAR ||
                     token.getType() == Parser::Token::Type::VAR_STRING)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    std::string temp(tk.asString());
                    std::string &name = temp;
                    CharSequence *n = runtime.getChars(name);
                    if (n == nullptr)
                        runtime.report("Not found the var named " + name, true);
                    return n->to_string();
                });
        } else throw syntax_exception("Token is not a number/real.");
    }

    Getter<std::string> warpToString(Parser::Token *token) {
        return warpToString(*token);
    }

    template<typename T>
    class WorkDclVar : virtual public VWork {
    protected:
        std::string name;
        T defaultValue;
    public:
        WorkDclVar(std::string name, T defaultValue) : name(std::move(name)), defaultValue(defaultValue) {}

        virtual void performance(VirtualMachine::Runtime &runtime) =0;
    };

    class WorkDclVarNumber : virtual public WorkDclVar<Number> {
    public:
        WorkDclVarNumber(std::string name, Number defaultValue) :
                WorkDclVar(std::move(name), defaultValue) {}

        virtual void performance(VirtualMachine::Runtime &runtime) {
            runtime.allocate(name, defaultValue);
        };
    };

    class WorkDclVarChar : virtual public WorkDclVar<Char> {
    public:
        WorkDclVarChar(std::string name, Char charSequence) :
                WorkDclVar(std::move(name), defaultValue) {}

        virtual void performance(VirtualMachine::Runtime &runtime) {
            runtime.allocate(name, defaultValue);
        };
    };

    class WorkDclVarString : virtual public WorkDclVar<String> {
    public:
        WorkDclVarString(std::string name, String charSequence) :
                WorkDclVar(std::move(name), defaultValue) {}

        virtual void performance(VirtualMachine::Runtime &runtime) {
            runtime.allocate(name, defaultValue);
        };
    };

    class WorkVarArgs : virtual public VWork {
    protected:
        std::string target;
        std::vector<Getter<Number>> src;

    public:
        WorkVarArgs(std::string target, const std::vector<Getter<Number>> &src) : target(std::move(target)),
                                                                                  src(std::move(src)) {}

        virtual void performance(VirtualMachine::Runtime &runtime)=0;
    };

    class WorkAdd : public WorkVarArgs {
    public:
        WorkAdd(std::string target, const std::vector<Getter<Number>> &src) : WorkVarArgs(std::move(target),
                                                                                          std::move(src)) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *n = runtime.getNumber(target);
            if (n != nullptr) {
                std::cout << "perfomance add" << std::endl;
                Number &ref = *n;
                for (Getter<Number> &get : src)
                    ref += get.get(runtime);
            } else {
                runtime.report("Runtime error. No such variable named" + target + ".", true);
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

        virtual ~WorkSub() {
        }

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *np = runtime.getNumber(target);
            if (np != nullptr)
                (*np) = a.get(runtime) - b.get(runtime);
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

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *np = runtime.getNumber(target);
            if (np != nullptr) {
                Number &&an = b.get(runtime);
                Number &&bn = a.get(runtime);
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

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *n = runtime.getNumber(target);
            if (n != nullptr) {
                Number &ref = *n;
                try {
                    for (Getter<Number> &get : src)
                        ref *= get.get(runtime);
                } catch (std::bad_exception e) {
                    runtime.report(&e, true);
                }
            } else {
                runtime.report("Runtime error. No such variable named" + target + ".", true);
            }
        }
    };

    template<typename T>
    class WorkAssign : public VWork {
        std::string to;
        std::string from;
    public:
        WorkAssign(std::string to, std::string from) : to(std::move(to)), from(std::move(from)) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            if (runtime.getNumber(to) != nullptr) {

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
            if (dynamic_cast<String * > (pt)) {
                return dynamic_cast<String * > (pt);
            } else {
                runtime.report("", true);
                return nullptr;
            }
        }
    };

    class WorkAssignChar : public WorkAssign<Char> {
    public:
        WorkAssignChar(const std::string &to, const std::string &from) : WorkAssign(to, from) {}

        virtual Char *get(std::string &name, VirtualMachine::Runtime &runtime) override {
            CharSequence *pt = runtime.getChars(name);
            if (pt == nullptr) return nullptr;
            if (dynamic_cast<Char * > (pt)) {
                return dynamic_cast<Char * > (pt);
            } else {
                runtime.report("", true);
                return nullptr;
            }
        }
    };

    class WorkOut : public VWork {
        std::vector<Getter<std::string>> getters;

    public:
        WorkOut(const std::vector<Getter<std::string>> &getters) : getters(getters) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            std::cout << "perfomance out" << std::endl;

            for (Getter<std::string> &getter :getters)
                *runtime.out() << getter.get(runtime);
        }
    };

    class WorkSetStrChar : public VWork {
        std::string varName;
        Getter<Number> idx;
        Getter<Char *> cha;
    public:
        WorkSetStrChar(const std::string &varName, const Getter<mis::Number> &idx, const Getter<mis::Char *> &cha)
                : varName(varName), idx(idx), cha(cha) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            CharSequence *p = runtime.getChars(varName);
            if (dynamic_cast<String *>(p)) {
                String *s = dynamic_cast<String *>(p);
                Number &&n = idx.get(runtime);
                if (n.isReal() || n.asNumeric() > s->length()) {

                } else {
                    s->setCharAt(n.asNumeric(), cha.get(runtime)->getChar());
                }
            }
        }
    };

    class WorkGetStrChar : public VWork {
        std::string varName;
        Getter<Number> idx;
        std::string targetVarName;
    public:
        WorkGetStrChar(const std::string &varName, const Getter<mis::Number> &idx, const std::string &targetVarName)
                : varName(varName), idx(idx), targetVarName(targetVarName) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            CharSequence *p = runtime.getChars(varName);
            if (dynamic_cast<String *>(p)) {
                String *s = dynamic_cast<String *>(p);
                Number n = idx.get(runtime);
                if (n.isReal() || n.asNumeric() > s->length()) {

                } else {
                    CharSequence *target = runtime.getChars(targetVarName);
                    if (dynamic_cast<String *>(target)) {

                    }
                }
            }
        }
    };

    class WorkSleep : public VWork {
        Getter<Number> time;
    public:
        WorkSleep(Getter<mis::Number> time) : time(std::move(time)) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number &&tm = time.get(runtime);
            if (tm.isNumeric())
                std::this_thread::sleep_for(std::chrono::duration<long>(time.get(runtime).asNumeric()));
            else std::this_thread::sleep_for(std::chrono::duration<double>(time.get(runtime).asReal()));
        }
    };

    class WorkPlaceHolder : virtual public VWork {
    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
        }
    };

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

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            runtime.getIterator() = itr;
        }
    };

    class WorkJumpZ : public WorkJump {
        Getter<Number> getter;
        bool nz;
    public:
        WorkJumpZ(const std::vector<mis::VirtualMachine::Work *>::iterator &itr, const Getter<mis::Number> &getter,
                  bool nz) : WorkJump(itr), getter(getter), nz(nz) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            bool isZ = getter.get(runtime) == 0;
            if (nz && !isZ)
                WorkJump::performance(runtime);
            else if (isZ)
                WorkJump::performance(runtime);
        }
    };

    class WorkJumpCompare : public WorkJump {
        Getter<Number> a, b;
        WorkPreJMPCOMP::Type T;
    public:
        WorkJumpCompare(const std::vector<mis::VirtualMachine::Work *>::iterator &itr, const Getter<mis::Number> &a,
                        const Getter<mis::Number> &b, WorkPreJMPCOMP::Type T) : WorkJump(itr), a(a), b(b), T(T) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            switch (T) {
                case WorkPreJMPCOMP::Type::GT:
                    if (a.get(runtime) > b.get(runtime))
                        WorkJump::performance(runtime);
                    break;
                case WorkPreJMPCOMP::Type::LT:
                    if (a.get(runtime) < b.get(runtime))
                        WorkJump::performance(runtime);
                    break;
                case WorkPreJMPCOMP::Type::GTE:
                    if (a.get(runtime) >= b.get(runtime))
                        WorkJump::performance(runtime);
                    break;
                case WorkPreJMPCOMP::Type::LTE:
                    if (a.get(runtime) <= b.get(runtime))
                        WorkJump::performance(runtime);
                    break;
            }
        }

    };


    inline void regFilter(Parser::Builder &builder);

    void registerDefault(Parser::Builder &builder) {
        regFilter(builder);

        builder.registerInstructionBuilder("VAR", constrain(
                [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                    VirtualMachine::Work *pt = nullptr;
                    std::string name(tokens[0]->asString());

                    if (context.find(name) != context.end()) {
                        throw syntax_exception(
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
                        pt = new WorkDclVarString(name,
                                                  String(tokens[2]->asString().data()));
                    return pt;
                }, TypeMatcher().after(TokenType::VAR_NAME).after(TokenType::TYPE).
                        after(RangedType() || TokenType::NUMBER || TokenType::REAL || TokenType::STRING ||
                              TokenType::CHAR)));
        RangedType numberOrVar = RangedType()
                                 || TokenType::NUMBER || TokenType::REAL
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
                                                       std::vector<Getter<Number>> src;
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
                                                       std::vector<Getter<Number>> src;
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

        builder.registerInstructionBuilder("SET_STR_CHAR",
                                           constrain(
                                                   [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                       return new WorkDiv(std::string(tokens[0]->asString()),
                                                                          wrap(tokens[1]), wrap(tokens[2]));
                                                   }, TypeMatcher().after(RangedType()
                                                                          || TokenType::VAR_NUMERIC ||
                                                                          TokenType::VAR_REAL)
                                                           .after(numberOrVar)
                                                           .after(numberOrVar)
                                           ));

        TypeMatcher wildCard = TypeMatcher()
                .after(VariantArguments<RangedType>(RangedType() ||
                                                    TokenType::VAR_NUMERIC || TokenType::VAR_REAL ||
                                                    TokenType::VAR_STRING || TokenType::VAR_CHAR ||
                                                    TokenType::CHAR || TokenType::NUMBER ||
                                                    TokenType::REAL || TokenType::STRING)
                               .setMin(1).setMax(12));
        builder.registerInstructionBuilder("OUT",
                                           constrain(
                                                   [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                       std::vector<Getter<std::string>> getter;
                                                       for (int i = 0; i < tokens.size(); ++i)
                                                           getter.push_back(warpToString(tokens[i]));
                                                       return new WorkOut(getter);
                                                   }, wildCard));
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
                                                           throw std::bad_exception();
                                                       if (bVar == context.end())
                                                           throw std::bad_exception();
                                                       if (*aVar != *bVar)
                                                           throw std::bad_exception();
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

        builder.registerInstructionBuilder("SLEEP",
                                           constrain(
                                                   [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                       return new WorkSleep(wrap(tokens[0]));
                                                   }, TypeMatcher().after(numberOrVar)));


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
                        *work = *(new WorkJump(++((*target).second)));
                    } else {
                        //not found
                        throw std::bad_exception();
                    }
                } else if (instanceof<WorkPreJMPCOMP, VWork>(work)) {
                    WorkPreJMPCOMP *jp = dynamic_cast<WorkPreJMPCOMP *>(work);
                    auto target = labelMap.find(jp->getLabel());
                    if (target != labelMap.end()) {
                        *work = *new WorkJumpCompare(((*target).second), jp->getA(), jp->getB(), jp->getTp());
                    } else {
                        //not found
                        throw std::bad_exception();
                    }
                } else if (instanceof<WorkPreJMPZ, VWork>(work)) {
                    WorkPreJMPZ *jp = dynamic_cast<WorkPreJMPZ *>(work);
                    auto target = labelMap.find(jp->getLabel());
                    if (target != labelMap.end()) {
                        *work = *new WorkJumpZ((*target).second, jp->getGetter(), jp->isIsNZ());
                    } else {
                        //not found
                        throw std::bad_exception();
                    }
                }
            }
        });
    }

    inline void regFilter(Parser::Builder &builder) {
        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse varname
            Parser::Token *pt = nullptr;

            if (str.at(0) == '$') {
                std::string real(str.substr(1));
                auto itr = context.find(real);
                if (itr != context.end()) {
                    auto type = (*itr).second;
                    if (type == _NUMERIC) {
                        context[real] = _NUMERIC;

                        pt = new Parser::Token(Parser::Token::Type::VAR_NUMERIC, real, Parser::Token::Data{});
                    } else if (type == _REAL) {
                        context[real] = _REAL;
                        pt = new Parser::Token(Parser::Token::Type::VAR_REAL, real, Parser::Token::Data{});
                    } else if (type == _CHAR) {
                        context[real] = _CHAR;
                        pt = new Parser::Token(Parser::Token::Type::VAR_CHAR, real, Parser::Token::Data{});
                    } else if (type == _STRING) {
                        context[real] = _STRING;

                        pt = new Parser::Token(Parser::Token::Type::VAR_STRING, real, Parser::Token::Data{});
                    } else throw syntax_exception("Internal error");//internal error
                } else {
                    if (context["%CURRENT_INST%"] == "VAR") {
                        pt = new Parser::Token(Parser::Token::Type::VAR_NAME, real, Parser::Token::Data{});
                    } else {
                        throw syntax_exception("no such var declared named [" + real + "]");
                    }
                }
            }
            return pt;
        });
        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse number
            Parser::Token *pt = nullptr;
            int tp = get_type(str);
            if (tp != -1) {
                if (tp == 0) {
                    long l = atol(str.c_str());
                    Parser::Token::Data d;
                    d.numeric = l;
                    pt = new Parser::Token(Parser::Token::Type::NUMBER, str, d);
                } else {
                    double v = atof(str.c_str());
                    Parser::Token::Data d;
                    d.real = v;
                    pt = new Parser::Token(Parser::Token::Type::NUMBER, str, d);
                }
            }
            return pt;
        });
        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse string/char
            Parser::Token *pt = nullptr;
            if (str[0] == '\'' && str[str.length() - 1] == '\'' && str.length() == 3) {
                Parser::Token::Data d;
                d.character = str[1];
                pt = new Parser::Token(Parser::Token::Type::CHAR, str, d);

            } else if (str[0] == '\"' && str[str.length() - 1] == '\"') {
                std::string temp(str);
                temp.erase(str.begin());
                temp.erase(str.end() - 1);
                Parser::Token::Data d;
                pt = new Parser::Token(Parser::Token::Type::STRING, str, d);
            }
            return pt;
        });
        //hard code... this is not good
        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse keyword(types)
            Parser::Token *pt = nullptr;
            if (str == _NUMERIC || str == _REAL || str == _CHAR || str == _STRING) {
                Parser::Token::Data d;
                pt = new Parser::Token(Parser::Token::Type::TYPE, str, d);
            }
            return pt;
        });

        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse other(labels)
            Parser::Token *pt = nullptr;
            Parser::Token::Data d;
            pt = new Parser::Token(Parser::Token::Type::PLAIN_TEXT, str, d);
            return pt;
        });
    }
}