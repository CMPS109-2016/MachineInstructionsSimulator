//
// Created by CIJhn on 10/30/2016.
//

#include <mis/syntax_exception.h>
#include <iostream>
#include "mis/Parser.h"
#include "mis/strutil.h"

namespace mis {

    VirtualMachine::Work *Parser::parseUnit(Context &context, const std::string &line) {

        std::string::size_type pos = line.find_first_of(" ");
        std::string inst(line, 0, pos);
        std::string arguments(line, pos + 1, line.length());

        auto itr = map.find(inst);

        if (itr != map.end()) {
            std::vector<std::string> buffer;
            UnitBuilder *ub = (*itr).second;
            const std::string &args = arguments;

            context["%CURRENT_INST%"] = inst;
            mis::splitDetectStringChar(args, ',', buffer);
            std::vector<Parser::Token *> tokens;
            for (std::string &s: buffer) {
                Token *tp = nullptr;
                for (Filter filter: filters) {
                    tp = filter(context, s);
                    if (tp != nullptr)
                        break;
                }
                if (tp == nullptr)
                    throw std::bad_exception();
                tokens.push_back(tp);
            }

            VirtualMachine::Work *wp = (*ub).build(context, tokens);
            for (Parser::Token *token:tokens)
                delete (token);
            return wp;
        } else
            throw syntax_exception("Not find such Instruction " + inst);
    }

    Parser::~Parser() {
        auto itr = map.begin();
        while (itr != map.end()) {
            delete ((*itr).second);
        }
    }

    std::vector<VirtualMachine::Work *> Parser::parse(const std::string &lines) {
        std::string temp(lines);
        trim(temp);
        std::vector<VirtualMachine::Work *> works;
        std::vector<std::string> buffer;
        mis::splitDetectStringChar(lines, '\n', buffer);
        std::map<std::string, std::string> *context = new std::map<std::string, std::string>();
        for (std::string &line:  buffer) {
            VirtualMachine::Work *work = parseUnit(*context, line);
            if (work != nullptr)
                works.push_back(work);
            else {
                std::cerr << "unable to parse line [" + line + "]" << std::endl;
            }
        }
        for (Linker link: linkers)
            link(works);
        delete (context);
        return works;
    }


    Parser::Parser(std::map<std::string, Parser::UnitBuilder *> &m, std::vector<Parser::Filter> &f) :
            map(m), filters(f) {
    }


    Parser::Token::Type Parser::Token::getType() const {
        return type;
    }

    const Parser::Token::Data &Parser::Token::getData() const {
        return data;
    }

    Parser::Token::~Token() {
    }

    Parser::Token::Token(const Parser::Token &tk) : type(tk.type) {
        data = tk.getData();
        raw = tk.raw;
    }

    const std::string &Parser::Token::asString() const {
        return raw;
    }

    Parser::Token::Token(Parser::Token::Type type, const std::string &raw, const Parser::Token::Data &data) : raw(raw),
                                                                                                              type(type) {
        this->data = data;
    }

    Parser *Parser::Builder::build() {
        return new Parser(map, filters);
    }

    void Parser::Builder::registerFilter(Parser::Filter filter) {
        filters.push_back(filter);
    }

    bool Parser::Builder::registerInstructionBuilder(const std::string &instruction, Parser::UnitBuilder *builder) {
        auto itr = map.find(instruction);
        if (itr != map.end())
            return false;
        this->map[instruction] = builder;
        return true;
    }

    void Parser::Builder::registerLinker(Parser::Linker linker) {
        linkers.push_back(linker);
    }
}


//printf("done filter:");
//            for (auto tk:tokens) {
//                Token::Type tp = tk->getType();
//                switch (tp) {
//                    case Token::Type::NUMBER:
//                        printf("NUMBER");
//                        break;
//                    case Token::Type::REAL:
//                        printf("REAL");
//                        break;
//                    case Token::Type::CHAR:
//                        printf("CHAR");
//                        break;
//                    case Token::Type::STRING:
//                        printf("STRING");
//                        break;
//                    case Token::Type::PLAIN_TEXT:
//                        printf("PL_T");
//                        break;
//                    case Token::Type::TYPE:
//                        printf("TYPE");
//                        break;
//                    case Token::Type::VAR_NAME:
//                        printf("V_NAME");
//                        break;
//                    case Token::Type::VAR_NUMERIC:
//                        printf("V_NUMERIC");
//                        break;
//                    case Token::Type::VAR_STRING:
//                        printf("V_STR");
//                        break;
//                    case Token::Type::VAR_CHAR:
//                        printf("V_CHR");
//                        break;
//                    case Token::Type::VAR_REAL:
//                        printf("V_REAL");
//                        break;
//                }
//                printf(" ");
//            }
//            printf("\n");