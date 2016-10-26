//
// Created by CIJhn on 10/23/2016.
//

#include <stdexcept>
#include "../includes/RestrictedExecutor.h"

using namespace mis;

void RestrictedExecutor::execute(std::vector<Token> &vars)throw(std::invalid_argument) {
    for (int i = 0; i < restrictions.size(); ++i) {
        Predicate &pre = restrictions[i];
        pre.validate(vars);
    }
    doExecute(vars);
}

RestrictedExecutor::RestrictedExecutor(std::vector<Predicate> &restrictions) : restrictions(
        restrictions) {
}

RestrictedExecutor::~RestrictedExecutor() {
    delete &restrictions;
}
