//
// Created by CIJhn on 10/30/2016.
//

#include "mis/VirtualMachine.h"
#include "mis/Parser.h"
#include "mis/strutil.h"


namespace mis {
    VirtualMachine::Work *Parser::parseUnit(const std::string &line) {
        std::vector<std::string> vec;
        mis::split(line, " ", vec);
        if (map.find(vec[0]) != map.end()) {
            UnitBuilder ub = map[vec[0]];
            const std::string &args = vec[1];
            vec.clear();
            mis::split(args, " ", vec);

            std::vector<Parser::Token> tokens;

            for (std::string &s: vec)
                for (Filter filter:filters) {
                    Token *tp = filter(s);
                    if (tp != nullptr)
                        tokens.push_back(*tp);
                }

            return ub(tokens);
        } else
            throw std::bad_exception();
    }

    Parser::~Parser() {}

    std::vector<VirtualMachine::Work *> Parser::parse(const std::string &lines) {
        std::vector<VirtualMachine::Work *> r;
        std::vector<std::string> buffer;
        mis::split(lines, "\n", buffer);
        for (std::string &line:  buffer)
            r.push_back(parseUnit(line));
        for (Linker link: linkers)
            link(r);
        return r;
    }


    Parser::Parser(std::map<std::string, Parser::UnitBuilder> &m, std::vector<Parser::Filter> &f) :
            map(std::move(m)), filters(std::move(f)) {}

    Parser::Token::Type Parser::Token::getType() const {
        return type;
    }

    const Parser::Token::Data &Parser::Token::getData() const {
        return data;
    }

    Parser::Token::Token(Parser::Token::Type type, Parser::Token::Data data) : type(type), data(data) {}

    Parser::Token::~Token() {
        if (this->type == Parser::Token::Type::STRING || this->type == Parser::Token::Type::LABEL ||
            this->type == Parser::Token::Type::VAR || this->type == Parser::Token::Type::TYPE) {
            delete (this->data.string);
        }
    }

    Parser Parser::Builder::build() {
        return Parser(map, filters);
    }

    void Parser::Builder::registerFilter(Parser::Filter filter) {
        filters.push_back(filter);
    }

    bool Parser::Builder::registerInstructionBuilder(const std::string &instruction, Parser::UnitBuilder builder) {
        if (map.find(instruction) != map.end())
            return false;
        map[instruction] = builder;
        return true;
    }

    void Parser::Builder::registerLinker(Parser::Linker linker) {
        linkers.push_back(linker);
    }
}