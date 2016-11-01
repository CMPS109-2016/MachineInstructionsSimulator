#include <iostream>
#include <functional>
#include "mis/strutil.h"
#include "mis/Parser.h"

class A {
    class B;
};

class A::B {

};

int main() {
    mis::Parser::Token::Data data;
    return 0;
}