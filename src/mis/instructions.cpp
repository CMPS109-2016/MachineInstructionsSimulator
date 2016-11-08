//
// Created by CIJhn on 11/4/2016.
//

#include <mis/parser.h>
#include <iostream>
#include <mis/strutil.h>

namespace mis {
//    std::function<Number *(VirtualMachine::Runtime &)> &&constant(Number *n) {
//        Number *temp = n;
//        return [temp](VirtualMachine::Runtime &runtime) {
//            return temp;
//        };
//    }
//
//    std::function<Number &(VirtualMachine::Runtime &)> &&var(std::string &s) {
//        return [s](VirtualMachine::Runtime &runtime) {
//            Number *n = runtime.getNumber(s);
//            if (n != nullptr) {
//                return *n;
//            } else {
//                runtime.report("", true);
//                return Numeric(0);
//            }
//        };
//    }


    template<typename T>
    using Getter = std::function<T(VirtualMachine::Runtime &)>;
    using NGetter = std::function<Number(VirtualMachine::Runtime &)>;
    using VWork = VirtualMachine::Work;

    class WorkVarArgs : virtual public VWork {
    protected:
        std::string &target;
        std::vector<NGetter *> src;
    public:
        virtual void performance(VirtualMachine::Runtime &runtime)=0;
    };

    class WorkAdd : public WorkVarArgs {
    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *n = runtime.getNumber(target);
            if (n != nullptr) {
                Number &ref = *n;
                for (NGetter *getter : src)
                    ref += (*getter)(runtime);
            } else {
//                runtime.report();
            }
        }
    };

    class WorkSub : public VWork {
        std::string &target;
        NGetter *a, *b;

    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *np = runtime.getNumber(target);
            if (np != nullptr)
                (*np) = (*a)(runtime) - (*b)(runtime);
        }
    };

    class WorkDiv : public VWork {
        std::string &target;
        NGetter *a, *b;

    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *np = runtime.getNumber(target);
            if (np != nullptr) {
                Number &&temp = (*b)(runtime);
                if (temp == 0) {

                } else
                    (*np) = (*a)(runtime) / temp;
            }
        }
    };

    class WorkMul : public WorkVarArgs {
    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
            Number *n = runtime.getNumber(target);
            if (n != nullptr) {
                Number &ref = *n;
                for (NGetter *getter : src)
                    ref *= (*getter)(runtime);
            } else {
//                runtime.report();
            }
        }
    };

    template<typename T>
    class WorkAssign : public VWork {
        std::string &to;
        std::string &from;
    public:
        WorkAssign(std::string &to, std::string &from) : to(to), from(from) {}

        virtual void performance(VirtualMachine::Runtime &runtime) override {
            *get(to, runtime) = *get(from, runtime);
        }

        virtual T *get(std::string &name, VirtualMachine::Runtime &runtime)=0;
    };

    class WorkAssignNumber : public WorkAssign<Number> {
    public:
        WorkAssignNumber(std::string &to, std::string &from) : WorkAssign(to, from) {}

        virtual Number *get(std::string &name, VirtualMachine::Runtime &runtime) override {
            return runtime.getNumber(name);
        }
    };

    class WorkAssignString : public WorkAssign<String> {
    public:
        WorkAssignString(std::string &to, std::string &from) : WorkAssign(to, from) {}

        virtual String *get(std::string &name, VirtualMachine::Runtime &runtime) override {
            CharSequence *pt = runtime.getChars(name);
            if (pt == nullptr) return nullptr;
            if (dynamic_cast<String * > (pt)) {
                return dynamic_cast<String * > (pt);
            } else {
                return nullptr;
            }
        }
    };

    class WorkAssignChar : public WorkAssign<Char> {
    public:
        WorkAssignChar(std::string &to, std::string &from) : WorkAssign(to, from) {}

        virtual Char *get(std::string &name, VirtualMachine::Runtime &runtime) override {
            CharSequence *pt = runtime.getChars(name);
            if (pt == nullptr) return nullptr;
            if (dynamic_cast<Char * > (pt)) {
                return dynamic_cast<Char * > (pt);
            } else {
                return nullptr;
            }
        }
    };

    class WorkOut : public VWork {
        std::vector<Getter<Base *> *> getters;

    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
            for (Getter<Base *> *getter :getters)
                *runtime.out() << (*getter)(runtime)->to_string();
        }
    };

    class WorkSetStrChar : public VWork {
        std::string &varName;
        Getter<Number *> *idx;
        Getter<Char *> *cha;
    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
            CharSequence *p = runtime.getChars(varName);
            if (dynamic_cast<String *>(p)) {
                String *s = dynamic_cast<String *>(p);
                Number *n = (*idx)(runtime);
                if (n->isReal() || n->asNumeric() > s->length()) {

                } else {
                    s->setCharAt(n->asNumeric(), (*cha)(runtime)->getChar());
                }
            }
        }
    };

    class WorkGetStrChar : public VWork {
        std::string &varName;
        Getter<Number *> *idx;
        std::string &targetVarName;
    public:
        virtual void performance(VirtualMachine::Runtime &runtime) override {
            CharSequence *p = runtime.getChars(varName);
            if (dynamic_cast<String *>(p)) {
                String *s = dynamic_cast<String *>(p);
                Number *n = (*idx)(runtime);
                if (n->isReal() || n->asNumeric() > s->length()) {

                } else {
                    CharSequence *target = runtime.getChars(targetVarName);
                    if (dynamic_cast<String *>(target)) {

                    }
                }
            }
        }
    };


    void reg(Parser::Builder &builder) {
        builder.registerFilter([](std::string &str) {
            mis::Parser::Token *pt = nullptr;
            if (str.at(0) == '$') {
                mis::Parser::Token::Data d;
                d.string = str.data();
                pt = new mis::Parser::Token(mis::Parser::Token::Type::CHAR, d);
            }
            return pt;
        });
        builder.registerFilter([](std::string &str) {
            mis::Parser::Token *pt = nullptr;
            int tp = mis::get_type(str);
            if (tp != -1) {
                if (tp == 0) {
                    long l = atol(str.c_str());
                    mis::Parser::Token::Data d;
                    d.numeric = l;
                    pt = new mis::Parser::Token(mis::Parser::Token::Type::NUMBER, d);
                } else {
                    double v = atof(str.c_str());
                    mis::Parser::Token::Data d;
                    d.real = v;
                    pt = new mis::Parser::Token(mis::Parser::Token::Type::NUMBER, d);
                }
            }
            return pt;
        });
        builder.registerFilter([](std::string &str) {
            mis::Parser::Token *pt = nullptr;
            if (str[0] == '\'' && str[str.length() - 1] == '\'' && str.length() == 3) {
                mis::Parser::Token::Data d;
                d.character = str[1];
                pt = new mis::Parser::Token(mis::Parser::Token::Type::CHAR, d);

            } else if (str[0] == '\"' && str[str.length() - 1] == '\"') {
                std::string temp(str);
                temp.erase(str.begin());
                temp.erase(str.end() - 1);
                mis::Parser::Token::Data d;
                d.string = temp.c_str();
                pt = new mis::Parser::Token(mis::Parser::Token::Type::STRING, d);
            }
            return pt;
        });
        //hard code... this is not good
        builder.registerFilter([](std::string &str) {
            mis::Parser::Token *pt = nullptr;
            if (str == "REAL" || str == "NUMERIC" || str == "CHAR" || str == "STRING") {
                mis::Parser::Token::Data d;
                d.string = str.c_str();
                pt = new mis::Parser::Token(mis::Parser::Token::Type::TYPE, d);
            }
            return pt;
        });

        builder.registerFilter([](std::string &str) {
            mis::Parser::Token *pt = nullptr;
            mis::Parser::Token::Data d;
            d.string = str.c_str();
            pt = new mis::Parser::Token(mis::Parser::Token::Type::PLAIN_TEXT, d);
            return pt;
        });

    }

}