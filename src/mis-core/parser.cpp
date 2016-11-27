//
// Created by CIJhn on 10/30/2016.
//

#include <iostream>

#include "mis-core/mis_exception.h"
#include "mis-core/Parser.h"
#include "mis-core/strutil.h"

namespace mis {

    VirtualMachine::Work *Parser::parseUnit(Context &context, const std::string &line) {

        std::vector<std::string> buffer;

        mis::splitDetectStringChar(line, ' ', buffer);
        if (buffer.size() > 2) {
            throw mis_exception("Illegal syntax on line[" + line + "]");
        }
        std::string inst(buffer[0]);
        std::string arguments;
        if (buffer.size() > 1)
            arguments = buffer[1];

        auto itr = map.find(inst);

        if (itr != map.end()) {
            buffer.clear();
            UnitBuilder *ub = (*itr).second;
            const std::string &args = arguments;

            context["%CURRENT_INST%"] = inst;
            mis::splitDetectStringChar(args, ',', buffer);
            std::vector<Parser::Token *> tokens;
            for (auto &&s: buffer) {
                Token *tp = nullptr;
                for (Filter filter: filters) {
                    std::string *str = new std::string(s);
                    tp = filter(context, *str);
                    delete (str);
                    if (tp != nullptr)
                        break;
                }
                if (tp == nullptr)
                    throw mis_exception("Internal runtime exception happened on parsing line [" + line + "]");
                tokens.push_back(tp);
            }

            VirtualMachine::Work *wp = (*ub).build(context, tokens);
            for (Parser::Token *token:tokens)
                delete (token);
            return wp;
        } else
            throw mis_exception("Not find such Instruction " + inst);
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
            try {
                VirtualMachine::Work *work = parseUnit(*context, line);
                if (work != nullptr)
                    works.push_back(work);
                else {
                    std::cerr << "unable to parse line [" + line + "]" << std::endl;
                }
            } catch (mis_exception e) {
                std::cerr << e.getError() << std::endl;
            }
        }
        for (Linker link: linkers)
            link(works);
        delete (context);
        return works;
    }

    Parser::Parser(const std::map<std::string, Parser::UnitBuilder *> &map, const std::vector<Parser::Filter> &filters,
                   const std::vector<Parser::Linker> &linkers) : map(map), filters(filters), linkers(linkers) {}

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
        return new Parser(map, filters, linkers);
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

