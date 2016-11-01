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

        friend class Builder;

        typedef VirtualMachine::Work *(*UnitBuilder)(std::vector<Parser::Token> &args);

        typedef Token *(*Filter)(const std::string &s);

        typedef void (*Linker)(std::vector<VirtualMachine::Work *> &works);

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

        struct Token {
            enum class Type {
                REAL, NUMERIC, CHAR, STRING, LABEL, VAR, TYPE
            };

            union Data {
                long numeric;
                double real;
                char character;
                char *string;
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
