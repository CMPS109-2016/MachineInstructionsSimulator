//
// Created by CIJhn on 10/23/2016.
//


#ifndef MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
#define MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H

#include <vector>
#include <thread>
#include <mutex>
#include "lang.h"
#include "Executor.h"

namespace mis {
    class Parser;

    class VirtualMachine : public Terminable {
    public:
        /**
         * Set the out stream of the vm.
         * */
        VirtualMachine &operator()(std::ostream &ostream);

        VirtualMachine &operator<<(std::string &&code);

        /**
         * stream a line of code to the vm buffer.
         * */
//        VirtualMachine &operator<<(const std::string &code);


        /**
         * End and execute the code in buffer.
         * */
        VirtualMachine &end();

        VirtualMachine& parseAndRun(std::string& s);
        /**
         * Execute the method that return this.
         * */
        VirtualMachine &operator<<(VirtualMachine &(VirtualMachine::*)());

        struct Work;

        struct Runtime {
            virtual std::ostream *out()=0;

            virtual std::vector<Work *>::iterator &getIterator()=0;

            virtual CharSequence *allocate(std::string &s, CharSequence &charSequence)=0;

            virtual Number *allocate(std::string &s, Number &n)=0;

            virtual Number *getNumber(std::string &s)=0;

            virtual CharSequence *getChars(std::string &s)=0;

            virtual void report(const std::string &errorMessage, bool exit = false)=0;

            virtual void report(const std::exception *exception, bool exit = false)=0;
        };

        struct Work {
            virtual void performance(Runtime &runtime)=0;
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
         * Create VM from a parser and a Executor.
         * */
        VirtualMachine(Parser *parser, Executor *executor);

    private:
        Parser *parser;
        Executor *executor;

        std::ostream *stream;
        std::vector<std::string> buff;
        std::string buffer;
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
