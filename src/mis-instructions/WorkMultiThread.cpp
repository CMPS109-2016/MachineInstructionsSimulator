//
// Created by CIJhn on 11/21/2016.
//

#include "mis-instructions/instructions.h"

namespace mis {
    class WorkSleep : public VWork {
        Getter<Number> time;
    public:
        WorkSleep(Getter<mis::Number> time) : time(std::move(time)) {}

        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            Number &&tm = time.get(runtime, <#initializer#>);
            if (tm.isNumeric())
                std::this_thread::sleep_for(std::chrono::duration<long>(time.get(runtime, <#initializer#>).asNumeric()));
            else std::this_thread::sleep_for(std::chrono::duration<double>(time.get(runtime, <#initializer#>).asReal()));
        }
    };

    class WorkThreadBegin : public WorkPlaceHolder {};

    class WorkThreadEnd : public WorkPlaceHolder {};

    class WorkPreLock : public WorkPlaceHolder {
        std::string lockName;
        bool unlock;
    public:
        WorkPreLock(const std::string &lockName, bool unlock) : lockName(lockName), unlock(unlock) {}

        const std::string &getLockName() const {
            return lockName;
        }

        bool isUnlock() const {
            return unlock;
        }
    };

    class WorkBarrier : public VWork {
    public:
        void performance(VirtualMachine::Runtime &runtime,
                         std::vector<mis::VirtualMachine::Work *>::iterator &itr) override {
            runtime.barrier();
        }
    };

    class WorkLock : public VWork {
        std::shared_ptr<std::mutex> mutex;
    public:
        WorkLock(const std::shared_ptr<std::mutex> &mutex) : mutex(mutex) {}

    public:
        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            mutex->lock();
        }
    };

    class WorkUnlock : public VWork {
        std::shared_ptr<std::mutex> mutex;
    public:
        WorkUnlock(const std::shared_ptr<std::mutex> &mutex) : mutex(mutex) {}

    public:
        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            mutex->unlock();
        }
    };

    class WorkMultiThread : public VWork {
        std::vector<VWork *> works;
    public:
        WorkMultiThread(const std::vector<VWork *> &works) : works(works) {}

    public:
        virtual void performance(VirtualMachine::Runtime &runtime, Flow flow) override {
            VirtualMachine::Runtime *runtimePt = &runtime;
            runtime.thread([this, runtimePt]() {
                            for (auto itr = works.begin(); itr != works.end(); itr++) {
                                (*itr)->performance(*runtimePt, itr);
                            }
                        }, <#initializer#>);
        }
    };


}

void mis::regMultiThread(Parser::Builder &builder) {
    RangedType varName = RangedType()
                         || TokenType::NUMERIC || TokenType::REAL
                         || TokenType::VAR_REAL ||
                         TokenType::VAR_NUMERIC;
    builder.registerInstructionBuilder("SLEEP",
                                       constrain(
                                               [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                                                   return new WorkSleep(wrap(tokens[0]));
                                               }, TypeMatcher().after(varName)));

    builder.registerInstructionBuilder("THREAD_BEGIN", constrain(
            [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                return new WorkThreadBegin();
            }, TypeMatcher()
    ));
    builder.registerInstructionBuilder("THREAD_END", constrain(
            [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                return new WorkThreadEnd();
            }, TypeMatcher()
    ));
    builder.registerInstructionBuilder("LOCK", constrain(
            [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                return new WorkPreLock(tokens[0]->asString(), false);
            }, TypeMatcher().after(varName)
    ));
    builder.registerInstructionBuilder("UNLOCK", constrain(
            [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                return new WorkPreLock(tokens[0]->asString(), true);
            }, TypeMatcher().after(varName)
    ));
    builder.registerInstructionBuilder("BARRIER", constrain(
            [](Parser::Context &context, std::vector<Parser::Token *> &tokens) {
                return new WorkBarrier();
            }, TypeMatcher()
    ));
    builder.registerLinker([](std::vector<VirtualMachine::Work *> &works) {
        bool inThread = false;
        std::vector<VirtualMachine::Work *> rebuild;
        std::vector<VirtualMachine::Work *> buffer;
        for (auto itr = works.begin(); itr != works.end(); itr++) {
            if (instanceof<WorkThreadBegin, VWork>(*itr)) {
                if (inThread)
                    throw mis_exception("Child thread cannot initialize a new thread.");
                inThread = true;
            } else if (instanceof<WorkThreadEnd, VWork>(*itr)) {
                if (!inThread)
                    throw mis_exception("Unpaired thread end!");
                inThread = false;
                rebuild.push_back(new WorkMultiThread(buffer));
                buffer.clear();
            } else if (inThread) {
                buffer.push_back(*itr);
            } else {
                rebuild.push_back(*itr);
            }
        }
        works = rebuild;
    });
    builder.registerLinker([](std::vector<VirtualMachine::Work *> &works) {
        std::map<std::string, std::shared_ptr<std::mutex>> lockMap;
        for (auto itr = works.begin(); itr != works.end(); itr++) {
            if (instanceof<WorkPreLock, VWork>(*itr)) {
                WorkPreLock *pre = dynamic_cast<WorkPreLock *>(*itr);

                std::shared_ptr<std::mutex> ptr = nullptr;
                auto pair = lockMap.find(pre->getLockName());
                if (pair != lockMap.end()) {
                    ptr = (*pair).second;
                } else {
                    if (pre->isUnlock())
                        throw mis_exception("Attempt to unlock a non-exist lock.");
                    lockMap[pre->getLockName()] = ptr = std::shared_ptr<std::mutex>(new std::mutex());
                }
                if (pre->isUnlock())
                    (*itr) = new WorkUnlock(ptr);
                else
                    (*itr) = new WorkLock(ptr);
            }
        }
    });
}