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

    class RuntimeImpl : virtual public VirtualMachine::Runtime {
    private:
        std::vector<VirtualMachine::Work *>::iterator itr, end;
        std::basic_ostream<char> *outstream;
        std::map<std::string, CharSequence *> charsPool;
        std::map<std::string, Number *> numberPool;
        std::vector<VirtualMachine::Work *> &works;

        std::mutex mutex;
        std::map<std::thread *, VirtualMachine::Work::Flow> threadsMap;
        std::map<std::thread::id, VirtualMachine::Work::Flow> threadFlowMap;

    public:
        RuntimeImpl(std::vector<VirtualMachine::Work *> &units,
                    std::ostream *input) : outstream(input), works(units) {
            end = works.end();
        }

        ~RuntimeImpl() {
            for (std::pair<std::string, Number *> pair: numberPool)delete (pair.second);
            for (std::pair<std::string, CharSequence *> pair: charsPool)delete (pair.second);
            for (std::pair<std::thread *, VirtualMachine::Work::Flow> pair:threadsMap) delete (pair.first);
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
                std::cerr << "Runtime exception:" << std::endl;
                std::cerr << e.getError() << std::endl;
            }
        }


        virtual std::ostream *out() { return outstream; };

        virtual void halt(const std::string &errorMessage, VirtualMachine::Work::Flow flow) override {
            mutex.lock();
            auto id = std::this_thread::get_id();
            *outstream << errorMessage << std::endl;
            if (exit) {
                itr = end;
            }
            mutex.unlock();
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

        virtual void thread(std::function<void()> function, VirtualMachine::Work::Flow flow) override {
//            if (mainThreadID == std::this_thread::get_id())
//          can only called from main thread. therefore this is thread-safe
            std::thread *th = new std::thread(function);
            threadsMap[th] = flow;
        }

        void barrier() override {
            for (std::pair<std::thread *, VirtualMachine::Work::Flow> pair:threadsMap)
                pair.first->join();
        }
    };


    void VirtualMachine::run(std::vector<VirtualMachine::Work *> &works, std::basic_ostream<char> *ostream) {
        RuntimeImpl runtime(works, ostream);
        runtime.start();
    }
};



