//
// Created by CIJhn on 10/23/2016.
//

#include "mis/VirtualMachine.h"
#include <iostream>
#include <map>
#include <mis/strutil.h>
#include <mis/syntax_exception.h>

namespace mis {
    class RuntimeImpl : virtual public VirtualMachine::Runtime {
    private:
        std::vector<VirtualMachine::Work *>::iterator itr, end;
        std::ostream *outS;
        std::map<std::string, CharSequence *> charsPool;
        std::map<std::string, Number *> numberPool;


    public:
        RuntimeImpl(std::vector<VirtualMachine::Work *> &&units,
                    std::ostream *input) : outS(input) {
            VirtualMachine::Runtime &runtime = *dynamic_cast<VirtualMachine::Runtime *> (this);
        }

        ~RuntimeImpl() {
            for (std::pair<std::string, Number *> pair: numberPool)delete (pair.second);
            for (std::pair<std::string, CharSequence *> pair: charsPool)delete (pair.second);
        }

        virtual std::ostream *out() { return outS; };

        virtual void report(const std::string &errorMessage, bool exit) override {
            *outS << errorMessage << std::endl;
            if (exit) {
                itr = end;
            }
        }

        virtual void report(const std::exception *exception, bool exit) override {
            *outS << exception->what() << std::endl;
            if (exit) {
                itr = end;
            }
        }

        virtual std::vector<VirtualMachine::Work *>::iterator &getIterator() override {
            return itr;
        }

        virtual CharSequence *allocate(std::string &s, CharSequence &charSequence) {
            mis::CharSequence *p = nullptr;
            if (dynamic_cast<mis::String *>(&charSequence))
                p = new mis::String(charSequence.getAsCharArray());
            else if (dynamic_cast<mis::Char *>(&charSequence))
                p = new mis::Char(charSequence.getAsCharArray()[0]);
            if (p && charsPool.find(s) == charsPool.end() && numberPool.find(s) == numberPool.end())
                charsPool[s] = p;
            return p;
        };

        virtual Number *allocate(std::string &s, Number &n) {
            mis::Number *p = new mis::Number(n);
            if (numberPool.find(s) == numberPool.end() && charsPool.find(s) == charsPool.end())
                numberPool[s] = p;
            return p;
        };

        virtual Number *getNumber(std::string &s) { return numberPool[s]; };

        virtual CharSequence *getChars(std::string &s) { return charsPool[s]; };
    };


};

//mis::VirtualMachine::Future<void> &mis::VirtualMachine::execute(mis::VirtualMachine::TASK task) {
//    mutex.lock();
//    std::thread *tp = new std::thread(task);
//    mutex.unlock();
//    mis::VirtualMachine::Future<void> future;
//    return future;
//}

//mis::VirtualMachine &mis::VirtualMachine::operator<<(const std::string &code) {
//    std::cout << code << std::endl;
//    if (buffer.empty())
//        buffer = code;
//    else
//        buffer += code;
////    if (*(--code.end()) != '\n')
////        buffer += "\n";
//    return *this;
//}

mis::VirtualMachine &mis::VirtualMachine::operator<<(std::string &&code) {
    std::cout << code << std::endl;
    mis::splitDetectStringChar(code, '\n', buff);
    return *this;
}

#include "mis/Parser.h"

mis::VirtualMachine &mis::VirtualMachine::parseAndRun(std::string &s) {
    try {
        std::vector<VirtualMachine::Work *> works = this->parser->parse(s);
        RuntimeImpl runtime(std::move(works), this->stream);
        auto itr = works.begin();
        while (itr != works.end()) {
            VirtualMachine::Work *wk = (*itr);
            wk->performance(runtime);
            if (itr != works.end())//be careful the iterator since work might have side effect.
                ++itr;
        }
    } catch (syntax_exception e) {
        (*stream) << e.getError() << stream->end;
    }

    return *this;
}

mis::VirtualMachine &mis::VirtualMachine::end() {
    if (!this->buffer.empty()) {
        const std::string lines = buffer;
//        std::thread([this, lines] {
        try {
            std::vector<VirtualMachine::Work *> works = parser->parse(lines);
            RuntimeImpl runtime(std::move(works), stream);
            auto itr = works.begin();
            while (itr != works.end()) {
                VirtualMachine::Work *wk = (*itr);
                wk->performance(runtime);
                if (itr != works.end())//be careful the iterator since work might have side effect.
                    ++itr;
            }
        } catch (syntax_exception e) {
            (*stream) << e.getError() << stream->end;
        }
//        });
    }
    return *this;
}

mis::VirtualMachine &mis::VirtualMachine::operator<<(mis::VirtualMachine &(VirtualMachine::*func)()) {
    return (this->*func)();
}

mis::VirtualMachine &mis::VirtualMachine::operator()(std::ostream &ostream) {
    stream = &ostream;
    return *this;
}

bool mis::VirtualMachine::isTerminated() {
    return false;
}

void mis::VirtualMachine::terminate() {
//    executor.terminate();
}

mis::VirtualMachine::VirtualMachine(mis::Parser *parser, mis::Executor *executor) : parser(parser),
                                                                                    executor(executor),
                                                                                    stream(&std::cout) {}


