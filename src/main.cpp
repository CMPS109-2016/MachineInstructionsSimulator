#include <iostream>
#include <functional>
#include "../includes/Instruction.h"


class A {
public:
    virtual void print() {
        std::cout << "A" << std::endl;
    }
};

template<class T>
class B : public A {
public:
    virtual void print() override {
        std::cout << "B" << std::endl;
    }

};

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << mis::to_string(mis::ADD) << std::endl;

    A *a = new A;

    B<int> *b = new B<int>;

    a = b;

    b = dynamic_cast<B<int> *>(a);

    b->print();
    return 0;
}