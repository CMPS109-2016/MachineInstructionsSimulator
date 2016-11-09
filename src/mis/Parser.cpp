//
// Created by CIJhn on 10/30/2016.
//

#include "mis/VirtualMachine.h"
#include "mis/Parser.h"
#include "mis/strutil.h"


namespace mis {
    VirtualMachine::Work *Parser::parseUnit(const std::string &line) {
        std::vector<std::string> vec;

        std::string::size_type pos = line.find_first_of(" ");
        std::string inst(line, 0, pos);
        std::string arguments(line, pos, line.length());

        if (map.find(inst) != map.end()) {
            UnitBuilder ub = map[inst];
            const std::string &args = arguments;

            std::string::size_type last = 0;
            int embracedState = 0;
            for (std::string::size_type current = 0; current < args.length(); ++current) {
                char c = args[current];
                if (c == ',' && embracedState == 0) {
                    vec.push_back(std::string(arguments, last, current - last));
                    last = current;
                } else if (c == '\'') {
                    if (embracedState == 0)
                        embracedState = -1;
                    else if (embracedState == -1)
                        embracedState = 0;
                    else {
                        //syntax error: "xxxx'
                        throw std::bad_exception();
                    }
                } else if (c == '\"') {
                    if (embracedState == 0)
                        embracedState = 1;
                    else if (embracedState == 1)
                        embracedState = 0;
                    else {
                        //syntax error: 'xxxx"
                        throw std::bad_exception();
                    }
                }
            }

            if (embracedState != 0) {
                throw std::bad_exception();
            }

            std::vector<Parser::Token> tokens;
            for (std::string &s: vec) {
                Token *tp = nullptr;
                for (Filter filter: filters) {
                    tp = filter(s);
                    if (tp != nullptr)
                        break;
                }
                if (tp == nullptr)
                    throw std::bad_exception();
            }

            return ub(tokens);
        } else
            throw std::bad_exception();
    }

    Parser::~Parser() {}

    std::vector<VirtualMachine::Work *> Parser::parse(const std::string &lines) {
        std::string temp(lines);
        trim(temp);
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

    Parser::Token::Token(Parser::Token::Type type, Parser::Token::Data data) : type(type), data(std::move(data)) {
    }

    Parser::Token::~Token() {
//        if (this->type == Parser::Token::Type::STRING || this->type == Parser::Token::Type::LABEL ||
//            this->type == Parser::Token::Type::VAR_N || this->type == Parser::Token::Type::TYPE) {
//        }
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