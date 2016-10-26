//
// Created by CIJhn on 10/23/2016.
//

#include "../includes/PredicateParameterSize.h"

using namespace mis;

void PredicateParameterSize::validate(std::vector<Token> &vars) throw(std::invalid_argument) {
    if (vars.size() < min || vars.size() > max)
        throw (new std::invalid_argument("The parameter size has to be greater than " + std::to_string(min) +
                                         " and less than " + std::to_string(max) + "!"));
}

PredicateParameterSize::~PredicateParameterSize() {

}

PredicateParameterSize::PredicateParameterSize(int min, int max) : max(max), min(min) {
}
