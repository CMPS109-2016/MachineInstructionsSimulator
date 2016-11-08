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

    std::string s = "abbbba";

    s.erase(s.begin());
    s.erase(s.end() - 1);
    std::cout << s << std::endl;
//    C c(1);
    return 0;
}
