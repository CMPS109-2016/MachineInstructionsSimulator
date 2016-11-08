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

        using Filter = std::function<mis::Parser::Token *( std::string &)>;

        using UnitBuilder = std::function<mis::VirtualMachine::Work *(std::vector<Parser::Token> &)>;

        using Linker = std::function<void(std::vector<VirtualMachine::Work *> &)>;

        ~Parser();

        std::vector<VirtualMachine::Work *> parse(const std::string &lines);

    private:
        std::map<std::string, UnitBuilder> map;
        std::vector<Filter> filters;
        std::vector<Linker> linkers;

        VirtualMachine::Work *parseUnit(const std::string &line);

        Parser(std::map<std::string, UnitBuilder> &map, std::vector<Filter> &filters);

    public:
        class Builder {
        public:
            Parser build();

            void registerFilter(Filter filter);

            bool registerInstructionBuilder(const std::string &instruction, UnitBuilder builder);

            void registerLinker(Linker combiner);

        private:
            std::vector<Filter> filters;
            std::map<std::string, UnitBuilder> map;
            std::vector<Linker> linkers;
        };

        class Token {
        public:
            enum class Type {
                NUMBER, CHAR, STRING, PLAIN_TEXT, TYPE
            };

            union Data {
                long numeric;
                double real;
                char character;
                const char* string;
            };

            Type getType() const;

            const Data &getData() const;

            Token(Type type, Data data);

            ~Token();

        private:
            Data data;
            Type type;
        };
    };
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_PARSER_H
