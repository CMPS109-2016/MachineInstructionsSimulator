//
// Created by CIJhn on 10/23/2016.
//


#ifndef MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
#define MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H

#include <vector>
#include <thread>
#include <mutex>
#include "lang.h"

namespace mis {
    class Parser;

    class VirtualMachine {
    public:
        template<typename T>
        struct Callback {
            virtual void call(T &v)=0;
        };

        VirtualMachine &operator()(std::ostream &ostream);

        VirtualMachine &operator<<(const std::string &code);

        VirtualMachine &operator<<(VirtualMachine &(*)());

        struct Work;

        class Runtime {
        public:

            friend struct Work;

            virtual const std::ostream &out() = 0;

            virtual std::vector<Work *>::iterator getIterator()=0;

            virtual void jumpTo(const std::string &s)=0;

            virtual void mark(std::string &s)=0;

            virtual CharSequence *allocate(std::string &s, CharSequence &charSequence)=0;

            virtual Number *allocate(std::string &s, Number &n)=0;

            virtual Number *getNumber(std::string &s)=0;

            virtual CharSequence *getChars(std::string &s)=0;
        };

        struct Work {
            virtual void performance(Runtime &runtime)=0;
        };

        template<typename T>
        class Future {
        public:
            T get();

            bool isDone();

            void atDone(Callback<T> callback);

            void atFailure(Callback<std::exception> callback);
        };

        Future<void> submit(std::vector<Work *> works);

        bool isTerminated();

        void terminate();

        VirtualMachine &end();

    private:
        Parser *parser;
        std::ostream *stream;
        std::string buffer;
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
