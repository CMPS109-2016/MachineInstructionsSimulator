#include <iostream>
#include <functional>
#include <mis/restriction.h>

#include "mis/strutil.h"
#include "mis/default_instructions.h"
#include <fstream>

mis::VirtualMachine *buildDefaultVM() {

    mis::Parser::Builder builder;
    mis::registerDefault(builder);
    mis::Parser *parser = builder.build();
    return new mis::VirtualMachine(parser, mis::Executor::newBlockExecutor());
}

void test() {
    std::string test("VAR $myint,NUMERIC,100\nADD $myint,$myint,100\nOUT $myint");
    mis::VirtualMachine &vm = *buildDefaultVM();
    vm(std::cout);
    vm.parseAndRun(test);
}

int main(int argc, char *argv[]) {
//    if (argc == 1) {
//        std::string s(mis::readFileToString(std::string(argv[0])));
//        mis::VirtualMachine &vm = *buildDefaultVM();
//        vm.parseAndRun(s);
//        delete (vm);
//    }
    test();
    return 0;
}






