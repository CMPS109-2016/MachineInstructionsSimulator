#include <iostream>
#include <functional>
#include "mis/strutil.h"
#include "mis/Parser.h"

class A {
public:
    int v;

    A(int v) : v(v) {
        std::cout << "A con" << std::endl;
    }
};

class B : virtual public A {
public:
    B(int v) : A(v) {
        std::cout << "B con" << std::endl;
    }
};

class C : public B, virtual public A {
public:
    C(int v) : A(v), B(v) {}
};

int main() {
    std::string s("a \"inside\n test\"\nc \'d\'");

    std::vector<std::string> vector;
    try {
        mis::splitDetectStringChar(s, '\n', vector);
        for (std::string t:vector) {
            std::cout << t << std::endl;
            std::cout << std::endl;
        }
    } catch (std::bad_exception e) {
        std::cout << "exception" << std::endl;
    }

//    C c(1);
    return 0;
}
