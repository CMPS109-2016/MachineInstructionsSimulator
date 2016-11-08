//
// Created by CIJhn on 11/5/2016.
//

#include "UnitBuilderRestriction.h"

using namespace mis;

//TypeMatcher &TypeMatcher::operator,(Parser::Token::Type type) {
//    return *new TypeMatcher();
//}
//
//TypeMatcher &TypeMatcher::operator,(TypeMatcher::RangedType types) {
//    return *new TypeMatcher();
//}

Restriction *TypeMatcher::operator~() {

    return nullptr;
}

TypeMatcher &TypeMatcher::operator,(std::function<bool(std::vector<Parser::Token>::iterator)> func) {
    args.push_back(&func);
    return *this;
}

TypeMatcher &TypeMatcher::operator,(Parser::Token::Type t) {
    std::function<bool(std::vector<Parser::Token>::iterator)> func = [t](std::vector<Parser::Token>::iterator itr) {
        return (*itr).getType() == t;
    };
    args.push_back(&func);
    return *this;
};

TypeMatcher::RangedType &::mis::operator||(Parser::Token::Type a, Parser::Token::Type b) {
    return *new TypeMatcher::RangedType();
}

TypeMatcher &&::mis::operator,(Parser::Token::Type a, Parser::Token::Type b) {
    TypeMatcher &&matcher = TypeMatcher();
    matcher, a, b;
    return std::move(matcher);
}

TypeMatcher &&::mis::operator,(Parser::Token::Type a, TypeMatcher::RangedType &r) {
    TypeMatcher &&matcher = TypeMatcher();
    matcher, a, r;
    return std::move(matcher);
}

bool TypeMatcher::VariantArguments::isRange() {
    return false;
}

TypeMatcher::RangedType &TypeMatcher::RangedType::operator||(Parser::Token::Type type) {
    return *this;
}

Restriction *::mis::operator~(Parser::Token::Type a) {
    return nullptr;
}

Restriction *::mis::operator~(TypeMatcher::RangedType a) {
    return nullptr;
}

TypeMatcher::VariantArguments &::mis::operator<=(Parser::Token::Type b, int size) {
    return *new TypeMatcher::VariantArguments();
}

TypeMatcher::VariantArguments &::mis::operator<=(TypeMatcher::RangedType &r, int size) {
    return *new TypeMatcher::VariantArguments();
}

Restriction *::mis::operator~(TypeMatcher::VariantArguments a) {
    return nullptr;
}


