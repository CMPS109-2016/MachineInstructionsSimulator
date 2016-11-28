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
    class VirtualMachine {
    public:
        class Work;

        struct Runtime {
            virtual std::ostream *out()=0;

            virtual void allocate(std::string &s, CharSequence &charSequence)=0;

            virtual void allocate(std::string &s, Number &n)=0;

            virtual Number *getNumber(std::string &s)=0;

            virtual CharSequence *getChars(std::string &s)=0;

            virtual void thread(std::function<void()>, std::vector<Work *>::iterator &flow)=0;

            virtual void barrier()=0;
        };

        struct Work {
            using Flow = std::vector<Work *>::iterator &;

            virtual void performance(Runtime &runtime, Flow itr)=0;
        };

        /**
       * Run the Works
       * */
        void run(std::vector<mis::VirtualMachine::Work *> &works, std::basic_ostream<char> *ostream);

    };


}

#endif //MACHINEINSTRUCTIONSSIMULATOR_VIRTUALMACHINE_H
