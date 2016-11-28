//
// Created by CIJhn on 11/21/2016.
//
#include "mis-instructions/instructions.h"
#include "mis-core/log.h"
namespace mis {
    class WorkOut : public VWork {
        std::vector<Getter<std::string>> getters;
        std::mutex mutex;

    public:
        WorkOut(const std::vector<Getter<std::string>> &getters) : getters(getters) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {

            mutex.lock();
            for (Getter<std::string> &getter :getters)
                *runtime.out() << getter.get(runtime, <#initializer#>);
            mutex.unlock();
        }
    };


}

void mis::regOut(Parser::Builder &builder) {
    TypeMatcher wildCard = TypeMatcher()
            .after(VariantArguments<RangedType>(RangedType() ||
                                                TokenType::VAR_NUMERIC || TokenType::VAR_REAL ||
                                                TokenType::VAR_STRING || TokenType::VAR_CHAR ||
                                                TokenType::CHAR || TokenType::NUMERIC ||
                                                TokenType::REAL || TokenType::STRING)
                           .setMin(1).setMax(12));

    builder.registerInstructionBuilder("OUT",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   std::vector<mis::Getter<std::string>> getter;
                                                   for (int i = 0; i < tokens.size(); ++i)
                                                       getter.push_back(warpToString(tokens[i]));
                                                   return new WorkOut(getter);
                                               }, wildCard));
}