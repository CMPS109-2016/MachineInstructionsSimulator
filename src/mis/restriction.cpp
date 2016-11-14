//
// Created by CIJhn on 11/5/2016.
//

#include <iostream>
#include "mis/restriction.h"

namespace mis {


    template<typename T>
    int VariantArguments<T>::getMax() const {
        return max;
    }

    template<typename T>
    int VariantArguments<T>::getMin() const {
        return min;
    }

    template<typename T>
    VariantArguments<T> &VariantArguments<T>::setMax(int max) {
        VariantArguments::max = max;
        return *this;
    }

    template<typename T>
    VariantArguments<T> &VariantArguments<T>::setMin(int min) {
        VariantArguments::min = min;
        return *this;
    }

    template<typename T>
    VariantArguments<T>::VariantArguments(const mis::VariantArguments<T> &another) :
            max(another.max), min(another.min) {
        type = another.type;
    }

    template<typename T>
    T VariantArguments<T>::getType() const {
        return type;
    }

    template<typename T>
    VariantArguments<T>::VariantArguments(T &ref) {
        this->type = ref;
    }

    template<typename T>
    VariantArguments<T>::VariantArguments(T &&ref) {
        this->type = ref;
    }

    RangedType &RangedType::operator||(Parser::Token::Type type) {
        allowedType.insert(type);
        return *this;
    }

    bool RangedType::contains(const Parser::Token::Type type) const {
        return allowedType.find(type) != allowedType.end();
    }

    TypeMatcher &TypeMatcher::after(VariantArguments<RangedType> &args) {
        return after(std::move(args));
    }

    TypeMatcher &TypeMatcher::after(VariantArguments<RangedType> &&args) {
        this->args.push_back([args](std::vector<Parser::Token *>::iterator itr, int current, int length) {
            int remain = length - current;
            if (remain <= args.getMax() && remain >= args.getMin()) {
                const RangedType &tp = args.getType();
                for (int i = 0; i < remain; ++i) {
                    if (!tp.contains((*itr)->getType()))
                        return false;
                }
                return true;
            }
            return false;
        });
        return *this;
    }

    TypeMatcher &TypeMatcher::after(VariantArguments<TokenType> &&args) {
        this->args.push_back([args](std::vector<Parser::Token *>::iterator itr, int current, int length) {
            int remain = length - current;
            if (remain <= args.getMax() && remain >= args.getMin()) {
                const TokenType &tp = args.getType();
                for (int i = 0; i < remain; ++i)
                    if (tp != (*itr)->getType())
                        return false;
                return true;
            }
            return false;
        });
        return *this;
    }

    TypeMatcher &TypeMatcher::after(VariantArguments<TokenType> &args) {
        return after(std::move(args));
    }

    TypeMatcher &TypeMatcher::after(RangedType &&arg) {
        std::function<bool(std::vector<Parser::Token *>::iterator, int, int)> func =
                [arg](std::vector<Parser::Token *>::iterator itr, int current, int length) {
                    return arg.contains((*itr)->getType());
                };
        this->args.push_back(func);
        return *this;
    }

    TypeMatcher &TypeMatcher::after(TokenType &&t) {
        args.push_back([t](std::vector<Parser::Token *>::iterator itr, int current, int length) {
            return (*itr)->getType() == t;
        });
        return *this;
    }

    TypeMatcher &TypeMatcher::after(RangedType &arg) {
        std::function<bool(std::vector<Parser::Token *>::iterator, int, int)> func =
                [arg](std::vector<Parser::Token *>::iterator itr, int current, int length) {
                    return arg.contains((*itr)->getType());
                };
        this->args.push_back(func);
        return *this;
    }

    TypeMatcher &TypeMatcher::after(TokenType &t) {
        args.push_back([t](std::vector<Parser::Token *>::iterator itr, int current, int length) {
            return (*itr)->getType() == t;
        });
        return *this;
    }

    bool TypeMatcher::match(std::vector<Parser::Token *> &vector) const {
        auto itr = vector.begin();
        for (int i = 0; i < args.size(); ++i) {
            auto arg = args[i];
            bool r = arg(itr, i, (int) vector.size());
            if (!r)
                return false;
            ++itr;
        }
        return true;
    }

    class UBImpl : public Parser::UnitBuilder {
        TypeMatcher predicate;
        UnitBuilderFunc call;

    public:
        UBImpl(TypeMatcher predicate, UnitBuilderFunc &call) : predicate(predicate), call(call) {}

    private:
        virtual VirtualMachine::Work *build(Parser::Context &context, std::vector<Parser::Token *> &vector) override {
            mis::VirtualMachine::Work *wp = nullptr;
            if (predicate.match(vector))
                wp = call(context, vector);
            else {
                std::cerr << "type mismatched!" << std::endl;
            }
            return wp;
        }
    };

    Parser::UnitBuilder *constrain(UnitBuilderFunc &&builder, TypeMatcher &predicate) {
        return new UBImpl(predicate, builder);
    }

    Parser::UnitBuilder *constrain(UnitBuilderFunc &&builder, TypeMatcher &&predicate) {
        return new UBImpl(predicate, builder);
    }
}
