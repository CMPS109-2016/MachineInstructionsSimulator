//
// Created by CIJhn on 11/21/2016.
//

#include "mis-instructions/instructions.h"

namespace mis {
    Getter<mis::Number> wrapRef(mis::Parser::Token
                                &token) {
        if (token.getType() == Parser::Token::Type::NUMERIC)
            return Getter<Number>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                return Number(tk.getData().numeric);
            });
        else if (token.getType() == Parser::Token::Type::REAL)
            return Getter<Number>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                return Number(tk.getData().real);
            });
        else if (token.getType() == Parser::Token::Type::VAR_NUMERIC ||
                 token.getType() == Parser::Token::Type::VAR_REAL)
            return Getter<Number>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                std::string temp(tk.asString());
                std::string &name = temp;
                Number *n = runtime.getNumber(name);
                if (n == nullptr) {
                    throw mis_exception("Not found the var named " + name);
                }
                return *n;
            });
        else throw mis_exception("Token is not a number/real.");
    };


    Getter<Number> wrap(Parser::Token *token) {
        return wrapRef(*token);
    }

    Getter<std::string> warpToStringRef(Parser::Token &token) {
        if (token.getType() != TokenType::PLAIN_TEXT && token.getType() != TokenType::TYPE &&
            token.getType() != TokenType::VAR_NAME) {
            if (token.getType() == Parser::Token::Type::NUMERIC)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    return std::to_string(tk.getData().numeric);
                });
            else if (token.getType() == Parser::Token::Type::REAL)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    return std::to_string(tk.getData().real);
                });
            else if (token.getType() == Parser::Token::Type::VAR_NUMERIC ||
                     token.getType() == Parser::Token::Type::VAR_REAL)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    std::string temp(tk.asString());
                    Number *n = runtime.getNumber(temp);
                    if (n == nullptr) {
                        throw mis_exception("Not found the var named [" + temp + "]");
                    }
                    std::string s(n->to_string());

                    return n->to_string();
                });
            else if (token.getType() == TokenType::STRING)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    return std::string(tk.asString());
                });
            else if (token.getType() == TokenType::CHAR)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    std::string s;
                    s.push_back(tk.getData().character);
                    return s;
                });
            else if (token.getType() == Parser::Token::Type::VAR_CHAR ||
                     token.getType() == Parser::Token::Type::VAR_STRING)
                return Getter<std::string>(token, [](VirtualMachine::Runtime &runtime, Parser::Token &tk) {
                    std::string temp(tk.asString());
                    std::string &name = temp;
                    CharSequence *n = runtime.getChars(name);
                    if (n == nullptr) {
                        throw mis_exception("Not found the var named " + name);
                    }
                    return n->to_string();
                });
        }
        throw mis_exception("Token is not a number/real.");
    }

    Getter<std::string> warpToString(Parser::Token *token) {
        return warpToStringRef(*token);
    }


    void regFilter(Parser::Builder &builder) {

        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse varname
            Parser::Token *pt = nullptr;

            if (str.at(0) == '$') {
                std::string real(str.substr(1));
                auto itr = context.find(real);
                if (itr != context.end()) {
                    auto type = (*itr).second;
                    if (type == _NUMERIC) {
                        context[real] = _NUMERIC;
                        pt = new Parser::Token(Parser::Token::Type::VAR_NUMERIC, real, Parser::Token::Data{});
                    } else if (type == _REAL) {
                        context[real] = _REAL;
                        pt = new Parser::Token(Parser::Token::Type::VAR_REAL, real, Parser::Token::Data{});
                    } else if (type == _CHAR) {
                        context[real] = _CHAR;
                        pt = new Parser::Token(Parser::Token::Type::VAR_CHAR, real, Parser::Token::Data{});
                    } else if (type == _STRING) {
                        context[real] = _STRING;

                        pt = new Parser::Token(Parser::Token::Type::VAR_STRING, real, Parser::Token::Data{});
                    } else throw mis_exception("Internal error");//internal error
                } else {
                    if (context["%CURRENT_INST%"] == "VAR") {
                        pt = new Parser::Token(Parser::Token::Type::VAR_NAME, real, Parser::Token::Data{});
                    } else {
                        throw mis_exception("[Getter] no such var declared named [" + real + "]");
                    }
                }
            }
            return pt;
        });
        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse number
            Parser::Token *pt = nullptr;
            int tp = get_type(str);
            if (tp != -1) {
                if (tp == 0) {
                    long l = atol(str.c_str());
                    Parser::Token::Data d;
                    d.numeric = l;
                    pt = new Parser::Token(Parser::Token::Type::NUMERIC, str, d);
                } else {
                    double v = atof(str.c_str());
                    Parser::Token::Data d;
                    d.real = v;
                    pt = new Parser::Token(Parser::Token::Type::NUMERIC, str, d);
                }
            }
            return pt;
        });
        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse string/char
            Parser::Token *pt = nullptr;
            if (str[0] == '\'' && str[str.length() - 1] == '\'' && str.length() == 3) {
                Parser::Token::Data d;
                d.character = str[1];
                std::string temp = str.substr(1, str.length() - 2);
                pt = new Parser::Token(Parser::Token::Type::CHAR, std::string(temp), d);
            } else if (str[0] == '\"' && str[str.length() - 1] == '\"') {
                std::string temp = str.substr(1, str.length() - 2);
                pt = new Parser::Token(Parser::Token::Type::STRING, std::string(temp), Parser::Token::Data());
            }
            return pt;
        });
        //hard code... this is not good
        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse keyword(types)
            Parser::Token *pt = nullptr;
            if (str == _NUMERIC || str == _REAL || str == _CHAR || str == _STRING) {
                pt = new Parser::Token(Parser::Token::Type::TYPE, str, Parser::Token::Data());
            }
            return pt;
        });

        builder.registerFilter([](Parser::Context &context, std::string &str) {//parse other(labels)
            Parser::Token *pt = nullptr;
            pt = new Parser::Token(Parser::Token::Type::PLAIN_TEXT, str, Parser::Token::Data());
            return pt;
        });
    }

    void register_default(Parser::Builder &builder) {
        mis::log("register the builder, v1.0");
        regFilter(builder);
        regDeclVar(builder);
        regMath(builder);
        regAssign(builder);
        regString(builder);
        regMultiThread(builder);//multi thread has priority than jump
        regJump(builder);
        regOut(builder);
    }

}

