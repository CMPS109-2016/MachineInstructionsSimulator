//
// Created by CIJhn on 10/30/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_PARSER_H
#define MACHINEINSTRUCTIONSSIMULATOR_PARSER_H

#include "VirtualMachine.h"
#include <vector>
#include <map>

namespace mis {
    class Parser {
    public:
        struct Token;

        /**
         * The context, which is just a map from string to string to record the detail(side-effect) of the parsing.
         * */
        using Context = std::map<std::string, std::string>;
        /**
         * The filter which is responsed to the first stage of the parsing, from string to Token.
         * */
        using Filter = std::function<mis::Parser::Token *(Context &context, std::string &)>;

        /**
         * The builder builds work unit from the tokens. Matched by instruction.
         * */
        struct UnitBuilder {
            virtual mis::VirtualMachine::Work *build(Context &,
                                                     std::vector<Parser::Token *> &)=0;
        };



        /**
         * Post worker/handler that is used to implement some advanced feature.(like jump or multi-thread).
         * */
        using Linker = std::function<void(std::vector<VirtualMachine::Work *> &)>;

        Parser(const std::map<std::string, UnitBuilder *> &map, const std::vector<Filter> &filters,
               const std::vector<Linker> &linkers);

        ~Parser();


        /**
         * Parse all the content into works.
         * */
        std::vector<VirtualMachine::Work *> parse(const std::string &lines);

    private:
        std::map<std::string, UnitBuilder *> map;
        std::vector<Filter> filters;
        std::vector<Linker> linkers;

        VirtualMachine::Work *parseUnit(std::map<std::string, std::string> &context, const std::string &line);
    public:

        /**
         * The builder class of the parser.
         * */
        class Builder {
        public:
            Parser *build();

            /**
             * Register the filter to the parser.
             * */
            void registerFilter(Filter filter);

            /**
            * Register the builder to the parser by instruction string matching.
            * */
            bool registerInstructionBuilder(const std::string &instruction, UnitBuilder *builder);

            /**
            * Register the linker(post-handler) to the parser.
            * */
            void registerLinker(Linker combiner);

        private:
            std::vector<Filter> filters;
            std::map<std::string, UnitBuilder *> map;
            std::vector<Linker> linkers;
        };

        /**
         * The tokem of parsed from argument.
         * */
        class Token {
        public:
            /**
             * The all possible types of the token.
             * */
            enum class Type {
                NUMERIC, REAL, CHAR, STRING, PLAIN_TEXT, TYPE, VAR_NAME, VAR_NUMERIC, VAR_STRING, VAR_CHAR, VAR_REAL
            };

            /**
             * Using union to store the real data type.(indicated by Token::Type)
             * */
            union Data {
                long numeric;
                double real;
                char character;
            };

            /**
             * Get the type of this token.
             * */
            Type getType() const;

            /**
             * Get the data of this token.
             * */
            const Data &getData() const;

            const std::string &asString() const;

            Token(Parser::Token::Type type, const std::string &raw, const Parser::Token::Data &data);

            Token(const Token &tk);

            ~Token();

        private:
            Data data;
            std::string raw;
            Type type;
        };
    };
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_PARSER_H
