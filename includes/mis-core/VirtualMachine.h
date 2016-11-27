//
// Created by CIJhn on 10/23/2016.
//


#ifndef MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
#define MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "lang.h"

namespace mis {
    class Parser;

    class VirtualMachine : public Terminable {
    public:
        /**
         * Set the out stream of the vm.
         * */
        VirtualMachine &operator()(std::basic_ostream<char> *ostream);

        VirtualMachine &operator<<(std::string &&code);

        /**
         * End and execute the code in buffer.
         * */
        VirtualMachine &end();

        /**
         * Parse and run the code.
         * */
        VirtualMachine &parseAndRun(std::string &s);

        /**
         * Execute the method that return this.
         * */
        VirtualMachine &operator<<(VirtualMachine &(VirtualMachine::*)());

        struct Work;

        struct Runtime {

//            template<typename T>
//            struct Variable {
//                std::atomic<T> &operator*();
//
//                Variable(T value) : value(value) {}
//
//            private:
//                std::atomic<T> value;
//            };

//            template
//            class Variable<Number>;
//
//            template
//            class Variable<CharSequence>;

            virtual std::ostream *out()=0;

            virtual std::vector<Work *>::iterator &getIterator()=0;

            virtual void allocate(std::string &s, CharSequence &charSequence)=0;

            virtual void allocate(std::string &s, Number &n)=0;

            virtual Number *getNumber(std::string &s)=0;

            virtual CharSequence *getChars(std::string &s)=0;

            virtual void thread(std::function<void(void)>)=0;

            virtual void barrier()=0;

            virtual void report(const std::string &errorMessage, bool exit = false)=0;

            virtual void report(const std::exception *exception, bool exit = false)=0;
        };

        struct Work {
            using Flow = std::vector<Work *>::iterator &;

            virtual void performance(Runtime &runtime, Flow itr)=0;
        };

        /**
         * Check the vm is terminated or not.
         * */
        virtual bool isTerminated() override;

        /**
         * Terminate the vm. If it was already been terminated, nothing happen.
         * */
        virtual void terminate() override;

        /**
         * Create VM from a parser.
         * */
        VirtualMachine(Parser *parser);

    private:
        Parser *parser;
//        Executor *executor;
        std::basic_ostream<char> *stream;

        std::vector<std::string> buff;
        std::string buffer;
    };


}

#endif //MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
