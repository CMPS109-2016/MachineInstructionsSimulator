//
// Created by CIJhn on 11/21/2016.
//
#include "mis-instructions/instructions.h"

namespace mis {

    class WorkSetStrChar : public VWork {
        std::string varName;
        Getter<Number> idx;
        Getter<Char> cha;
    public:
        WorkSetStrChar(const std::string &varName, const Getter<mis::Number> &idx, const Getter<mis::Char> &cha)
                : varName(varName), idx(idx), cha(cha) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            CharSequence *p = runtime.getChars(varName);
            if (dynamic_cast<String *>(p)) {
                String *s = dynamic_cast<String *>(p);
                Number n = idx.get(runtime, <#initializer#>);
                if (n.isReal())
                    throw mis_exception("SET_STR_CHAR require the index being a Numeric.");
                if (n.asNumeric() > s->length())
                    throw mis_exception("SET_STR_CHAR the index should not greater than the length of the string.");
                s->setCharAt((int) n.asNumeric(), cha.get(runtime, <#initializer#>).getChar());
            }
        }
    };

    class WorkGetStrChar : public VWork {
        std::string varName;
        Getter<Number> idx;
        std::string targetVarName;
    public:
        WorkGetStrChar(const std::string &varName, const Getter<mis::Number> &idx, const std::string &targetVarName)
                : varName(varName), idx(idx), targetVarName(targetVarName) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            CharSequence *p = runtime.getChars(varName),
                    *charP = runtime.getChars(targetVarName);
            if (dynamic_cast<String *>(p) && dynamic_cast<Char *>(charP)) {
                String *s = dynamic_cast<String *>(p);
                Char *c = dynamic_cast<Char *>(charP);
                Number n = idx.get(runtime, <#initializer#>);
                if (n.isReal())
                    throw mis_exception("GET_STR_CHAR require the index being a Numeric.");
                if (n.asNumeric() > s->length())
                    throw mis_exception("GET_STR_CHAR the index should not greater than the length of the string.");
                *c = Char(s->getCharAt((int) n.asNumeric()));
            }
        }
    };


}

void mis::regString(Parser::Builder &builder) {
    builder.registerInstructionBuilder(
            "SET_STR_CHAR",
            constrain(
                    [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                        return new WorkSetStrChar(std::string(tokens[0]->asString()),
                                                  wrap(tokens[1]),
                                                  Getter<Char>(*tokens[2],
                                                               [](VirtualMachine::Runtime &rt,
                                                                  Parser::Token &tk) {
                                                                   if (tk.getType() == TokenType::CHAR)
                                                                       return tk.getData().character;
                                                                   else if (tk.getType() == TokenType::VAR_CHAR) {
                                                                       std::string n = tk.asString();
                                                                       CharSequence *cp = rt.getChars(n);
                                                                       if (dynamic_cast<Char * >(cp)) {
                                                                           return (dynamic_cast<Char * >(cp))->getChar();
                                                                       }
                                                                   }
                                                                   throw mis_exception(
                                                                           "char type mismatched on SET_STRING_CHAR");
                                                               }));
                    }, TypeMatcher().after(TokenType::VAR_STRING)
                            .after(RangedType() || TokenType::VAR_NUMERIC || TokenType::NUMERIC)
                            .after(RangedType() || TokenType::VAR_CHAR || TokenType::CHAR)
            ));

    builder.registerInstructionBuilder("GET_STR_CHAR",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkGetStrChar(std::string(tokens[0]->asString()),
                                                                             wrap(tokens[1]),
                                                                             std::string(tokens[2]->asString()));
                                               }, TypeMatcher().after(TokenType::VAR_STRING)
                                                       .after(RangedType() || TokenType::VAR_NUMERIC ||
                                                              TokenType::NUMERIC)
                                                       .after(RangedType() || TokenType::VAR_CHAR)
                                       ));

}
