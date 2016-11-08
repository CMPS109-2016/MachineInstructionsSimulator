//
// Created by CIJhn on 11/5/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_UNITBUILDERRESTRICTED_H
#define MACHINEINSTRUCTIONSSIMULATOR_UNITBUILDERRESTRICTED_H

#include <set>
#include "parser.h"

namespace mis {
    using Restriction = Predicate<std::vector<Parser::Token> &>;
    using TokenType = Parser::Token::Type;

    //try some grammar sugar.... though the c++ has few support on handling.... multi-args
    //like {a, b, c} or (a, b, c)
    //only the << pattern works fine(looks like builder pattern)

    //matcher += a += b += (range(), c , d),
    class TypeMatcher {
    public:
        class RangedType : public std::function<bool(std::vector<Parser::Token>::iterator)> {
        public :
            virtual RangedType &operator||(Parser::Token::Type type);

        private:
            std::set<Parser::Token::Type> allowedType;
        };

        class VariantArguments : public std::function<bool(std::vector<Parser::Token>::iterator)> {
            int max, min;
            union {
                RangedType *ranged;
                Parser::Token::Type single;
            } type;
            bool inRanged;

        public:
            bool isRange();
        };

        virtual TypeMatcher &operator,(std::function<bool(std::vector<Parser::Token>::iterator)>);

        virtual TypeMatcher &operator,(Parser::Token::Type);

        virtual Restriction *operator~();

    private:
        std::vector<std::function<bool(std::vector<Parser::Token>::iterator)> *> args;
    };

    inline TypeMatcher::VariantArguments &operator<=(Parser::Token::Type b, int size);

    inline TypeMatcher::VariantArguments &operator<=(TypeMatcher::RangedType &r, int size);

    inline Restriction *operator~(Parser::Token::Type a);

    inline Restriction *operator~(TypeMatcher::RangedType a);

    inline Restriction *operator~(TypeMatcher::VariantArguments a);

    inline TypeMatcher::RangedType &operator||(Parser::Token::Type a, Parser::Token::Type b);

    inline TypeMatcher &&operator,(Parser::Token::Type a, Parser::Token::Type b);

    inline TypeMatcher &&operator,(Parser::Token::Type a, TypeMatcher::RangedType &r);

    void test() {
//        Restriction *addRes = ~(TokenType::VAR_N, (TokenType::NUMBER || TokenType::VAR_N) <= 12);
//        Restriction *subRes = ~(TokenType::VAR_N, (TokenType::NUMBER || TokenType::VAR_N),
//                (TokenType::NUMBER || TokenType::VAR_N));
//        Restriction *r = ~(TokenType::CHAR <= 5, (TokenType::CHAR || TokenType::STRING));
//        r = ~Parser::Token::Type::CHAR;
//        r = ~(Parser::Token::Type::CHAR || Parser::Token::Type::STRING);
    }

    Parser::UnitBuilder wrap(Parser::UnitBuilder builder, Predicate<std::vector<Parser::Token> &> &predicate);
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_UNITBUILDERRESTRICTED_H
