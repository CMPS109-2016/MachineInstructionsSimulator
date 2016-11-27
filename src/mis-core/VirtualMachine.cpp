//
// Created by CIJhn on 10/23/2016.
//

#include "mis-core/VirtualMachine.h"
#include <iostream>
#include <map>
#include <atomic>
#include <set>
#include "mis-core/strutil.h"
#include "mis-core/mis_exception.h"

#include "mis-core/parser.h"

namespace mis {

//    template<typename T>
//    std::atomic<T> &VirtualMachine::Runtime::Variable::operator*() {
//        return value;
//    }

    class RuntimeImpl : virtual public VirtualMachine::Runtime {
    private:
        std::vector<VirtualMachine::Work *>::iterator itr, end;
        std::basic_ostream<char> *outstream;
        std::map<std::string, CharSequence *> charsPool;
        std::map<std::string, Number *> numberPool;
        std::vector<VirtualMachine::Work *> &works;

        std::mutex mutex;
        std::vector<std::thread *> threads;

    public:
        RuntimeImpl(std::vector<VirtualMachine::Work *> &units,
                    std::ostream *input) : outstream(input), works(units) {
            end = works.end();
        }

        ~RuntimeImpl() {
            for (std::pair<std::string, Number *> pair: numberPool)delete (pair.second);
            for (std::pair<std::string, CharSequence *> pair: charsPool)delete (pair.second);
        }

        void start() {
            itr = works.begin();
            try {
                while (itr != end) {
                    VirtualMachine::Work *wk = (*itr);
                    wk->performance(*this, itr);
                    if (itr != end)//be careful the iterator since work might have side effect.
                        ++itr;
                }
            }
            catch (mis_exception e) {
                std::cerr << e.getError() << std::endl;
            }
        }


        virtual std::ostream *out() { return outstream; };

        virtual void report(const std::string &errorMessage, bool exit) override {
            *outstream << errorMessage << std::endl;
            if (exit) {
                itr = end;
            }
        }

        virtual void report(const std::exception *exception, bool exit) override {
            *outstream << exception->what() << std::endl;
            if (exit) {
                itr = end;
            }
        }

        virtual std::vector<VirtualMachine::Work *>::iterator &getIterator() override {
            return itr;
        }

        virtual void allocate(std::string &s, CharSequence &charSequence) {
            mis::CharSequence *p = nullptr;
            if (dynamic_cast<mis::String *>(&charSequence))
                p = new mis::String(charSequence.getAsCharArray());
            else if (dynamic_cast<mis::Char *>(&charSequence))
                p = new mis::Char(charSequence.getAsCharArray()[0]);

            mutex.lock();
            charsPool[s] = p;
            mutex.unlock();
        };

        virtual void allocate(std::string &s, Number &n) {
            mis::Number *p = new mis::Number(n);
            mutex.lock();
            numberPool[s] = p;
            mutex.unlock();
        };

        virtual Number *getNumber(std::string &s) {
            mutex.lock();
            Number *pt = numberPool[s];
            mutex.unlock();
            return pt;
        };

        virtual CharSequence *getChars(std::string &s) {
            mutex.lock();
            CharSequence *pt = charsPool[s];
            mutex.unlock();
            return pt;
        };

        virtual void thread(std::function<void()> function) override {
//            if (mainThreadID == std::this_thread::get_id())
//          can only called from main thread. therefore this is thread-safe
            threads.push_back(new std::thread(function));
        }

        void barrier() override {
            for (auto thread : threads)
                thread->join();
        }
    };


};

//mis-core-core::VirtualMachine::Future<void> &mis-core-core::VirtualMachine::execute(mis-core-core::VirtualMachine::TASK task) {
//    mutex.lock();
//    std::thread *tp = new std::thread(task);
//    mutex.unlock();
//    mis-core::VirtualMachine::Future<void> future;
//    return future;
//}

//mis-core-core::VirtualMachine &mis-core-core::VirtualMachine::operator<<(const std::string &code) {
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


mis::VirtualMachine &mis::VirtualMachine::parseAndRun(std::string &s) {
    std::vector<VirtualMachine::Work *> works = this->parser->parse(s);
    RuntimeImpl runtime(works, this->stream);
    runtime.start();
    return *this;
}

mis::VirtualMachine &mis::VirtualMachine::end() {
    if (!this->buffer.empty()) {
        const std::string lines = buffer;
//        std::thread([this, lines] {
        std::vector<VirtualMachine::Work *> works = parser->parse(lines);
        RuntimeImpl runtime(works, stream);
        runtime.start();
//        });
    }
    return *this;
}

mis::VirtualMachine &mis::VirtualMachine::operator<<(mis::VirtualMachine &(VirtualMachine::*func)()) {
    return (this->*func)();
}

mis::VirtualMachine &mis::VirtualMachine::operator()(std::basic_ostream<char> *ostream) {
    stream = ostream;
    return *this;
}

//unimplemented, waiting for multi-thread
bool mis::VirtualMachine::isTerminated() {
    return false;
}

//unimplemented, waiting for multi-thread
void mis::VirtualMachine::terminate() {
//    executor.terminate();
}

mis::VirtualMachine::VirtualMachine(mis::Parser *parser) : parser(parser),
        /*executor(executor),*/
                                                           stream(&std::cout) {}


