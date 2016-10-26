//
// Created by CIJhn on 10/24/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_TOKENIZER_H
#define MACHINEINSTRUCTIONSSIMULATOR_TOKENIZER_H

#include <vector>
#include <string>
#include <map>
#include "tokens"

namespace mis {
    class Tokenizer {
    private:

    public:
        std::vector<Token> &token(const std::string line);

        Tokenizer();
    };
}


#endif //MACHINEINSTRUCTIONSSIMULATOR_TOKENIZER_H
