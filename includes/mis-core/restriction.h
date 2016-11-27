//
// Created by CIJhn on 11/5/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_UNITBUILDERRESTRICTED_H
#define MACHINEINSTRUCTIONSSIMULATOR_UNITBUILDERRESTRICTED_H

#include <set>
#include "parser.h"

namespace mis {
    /**
     * Shortcut for token type.
     * */
    using TokenType = Parser::Token::Type;
    /**
     * Used for lambda.
     * */
    using UnitBuilderFunc = std::function<mis::VirtualMachine::Work *(Parser::Context &,
                                                                      std::vector<Parser::Token *> &)>;

    /**
     * The arguments that have varaint length.
     * */
    template<typename T>
    class VariantArguments {
    private:
        int max, min;
        T type;

    public:
        VariantArguments<T> &setMax(int max);

        VariantArguments<T> &setMin(int min);

        T getType() const;

        /**
         * The maximum numbers of the arguments.
         * */
        int getMax() const;

        /**
         * The minimum numbers of the arguments.
         * */
        int getMin() const;

        VariantArguments(T &ref);

        VariantArguments(T &&ref);

        VariantArguments(const VariantArguments<T> &);

//        virtual ~VariantArguments();
    };

    /**
     * The class indecates several avaiable types.
     * */
    class RangedType {
    public :
        virtual RangedType &operator||(Parser::Token::Type type);

        /**
         * If a certain type is legal in this case.
         * */
        bool contains(const Parser::Token::Type type) const;

    private:
        std::set<Parser::Token::Type> allowedType;
    };

    template
    class VariantArguments<RangedType>;

    template
    class VariantArguments<TokenType>;

    /**
     * The type matcher used to detect the arg-type of the code.
     */
    class TypeMatcher {
    public:
        TypeMatcher &after(VariantArguments<RangedType> &&);

        TypeMatcher &after(VariantArguments<TokenType> &&);

        TypeMatcher &after(RangedType &&);

        TypeMatcher &after(TokenType &&);

        TypeMatcher &after(VariantArguments<RangedType> &);

        TypeMatcher &after(VariantArguments<TokenType> &);

        TypeMatcher &after(RangedType &);

        TypeMatcher &after(TokenType &);

        bool match(std::vector<Parser::Token *> &v) const;

    private:
        std::vector<std::function<bool(std::vector<Parser::Token *>::iterator, int, int)>> args;
    };

    /**
     * Build a new UnitBuilder by constrain an unit builder function
     * */
    Parser::UnitBuilder *constrain(UnitBuilderFunc &&builder, TypeMatcher &predicate);

    /**
     * Build a new UnitBuilder by constrain an unit builder function
     * */
    Parser::UnitBuilder *constrain(UnitBuilderFunc &&builder, TypeMatcher &&predicate);
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_UNITBUILDERRESTRICTED_H
