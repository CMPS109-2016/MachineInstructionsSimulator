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
        VirtualMachine &operator()(std::ostream &ostream);

        VirtualMachine &operator<<(const std::string &code);

        VirtualMachine &operator<<(VirtualMachine &(*)());

        struct Work;

        struct Runtime {
            virtual std::ostream *out()=0;

            virtual std::vector<Work *>::iterator getIterator()=0;

            virtual CharSequence *allocate(std::string &s, CharSequence &charSequence)=0;

            virtual Number *allocate(std::string &s, Number &n)=0;

            virtual Number *getNumber(std::string &s)=0;

            virtual CharSequence *getChars(std::string &s)=0;

            virtual void report(const std::string &errorMessage, bool exit = false)=0;
        };

        struct Work {
            virtual void performance(Runtime &runtime)=0;
        };

        void run(std::vector<Work *> works);

        virtual bool isTerminated() override;

        virtual void terminate() override;

        VirtualMachine &end();

        VirtualMachine(Parser *parser, Executor *executor);

    private:
        Parser *parser;
        Executor *executor;

        std::ostream *stream;
        std::string buffer;
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
