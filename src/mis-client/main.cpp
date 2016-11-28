#include <iostream>
#include <functional>
#include "mis-core/restriction.h"
#include "mis-core/strutil.h"
#include "mis-instructions/default_instructions.h"
#include <fstream>
#include "mis-core/log.h"

mis::VirtualMachine *buildDefaultVM() {

    mis::Parser::Builder builder;
    mis::register_default(builder);
    mis::Parser *parser = builder.build();
    return new mis::VirtualMachine(parser);
}

void test() {//should print 200
    std::string test("VAR $myint,NUMERIC,100\nADD $myint,$myint,100\nOUT $myint");
    mis::VirtualMachine &vm = *buildDefaultVM();
    vm(&std::cout);
    vm.parseAndRun(test);
}

void testSetStr() {//should print aba
    std::string test("VAR $var,STRING,\"aaa\"\nSET_STR_CHAR $var,1,\'b\'\nOUT $var");
    mis::VirtualMachine &vm = *buildDefaultVM();
    vm(&std::cout);
    vm.parseAndRun(test);
}

void testGetStr() {//should print a
    std::string test("VAR $var,STRING,\"aaa\"\n"
                             "VAR $char,CHAR,'c'\n"
                             "GET_STR_CHAR $var,1,$char\n"
                             "OUT $char");
    mis::VirtualMachine &vm = *buildDefaultVM();
    vm(&std::cout);
    vm.parseAndRun(test);
}

void testJump() {
    std::string test("VAR $var,NUMERIC,0\n"
                             "JMPZ l0,$var\n"
                             "OUT \"this should not happen\n\"\n"
                             "LABEL l0\n"
                             "OUT \"this should be printed.\n\"");
    mis::VirtualMachine &vm = *buildDefaultVM();
    vm(&std::cout);
    vm.parseAndRun(test);
}

void testNumber() {
    mis::log("test number");
    mis::Number n(10L);
    mis::Number b(20L);
    mis::Number r = n + b;
    mis::log(r.to_string());
}


void take(std::vector<int>::iterator &aref, std::vector<int>::iterator &refB) {
    aref = refB;
}

void testJMP() {
    std::vector<int> vector;
    for (int i = 0; i < 10; ++i) {
        vector.push_back(i);
    }

    std::vector<std::vector<int>::iterator> itrs;
    for (auto itr = vector.begin(); itr != vector.end(); itr++) {
        if (*itr % 2 == 0) {
            itrs.push_back(itr);
        }
    }

    int count = 0;
    for (auto itr = vector.begin(); itr != vector.end(); itr++) {
        if (count == 0)
            take(itr, itrs[3]);
        std::cout << std::to_string(count++) << std::endl;
    }
}


void testDepth() {
    mis::log("s");
}

void testMultiThread() {
    std::string test("THREAD_BEGIN\n"
                             "SLEEP 5\n"
                             "OUT \"From thread A 1\n\"\n"
                             "OUT \"From thread A 2\n\"\n"
                             "THREAD_END\n"
                             "THREAD_BEGIN\n"
                             "SLEEP 4\n"
                             "OUT \"From thread B 1\n\"\n"
                             "OUT \"From thread B 2\n\"\n"
                             "THREAD_END\n"
                             "BARRIER");
    mis::VirtualMachine &vm = *buildDefaultVM();
    vm(&std::cout);
    vm.parseAndRun(test);
}

int main(int argc, char *argv[]) {
//    testJMP();
//    test();
//    testSetStr();
//    std::cout << std::endl;
//    testGetStr();
    testMultiThread();
//    testNumber();
//    if (argc >= 1) {
//        std::string s(mis::readFileToString(std::string(argv[0])));
//        std::basic_ostream<char> *out = &std::cout;
//        if (argc == 2) {
//            std::ofstream ofstream;
//            ofstream.open(argv[1]);
//            out = &ofstream;
//        }
//        mis::VirtualMachine &vm = *buildDefaultVM();
//        vm(out);
//        vm.parseAndRun(s);
//        delete (&vm);
//    } else {
//        std::cout << "Please put an first argument as the input code file. misvm [filename] [outfile]" << std::endl;
//    }
//    return 0;
}






