//
// Created by CIJhn on 10/23/2016.
//

#include "mis/VirtualMachine.h"
#include <iostream>


namespace mis {
    class RuntimeImpl : virtual public VirtualMachine::Runtime {
    private:
        std::vector<Work *> units;
        const std::ostream *outS;
        std::map<std::string, CharSequence *> charsPool;
        std::map<std::string, Number *> numberPool;
        std::map<std::string, int> labelMap;
        int current;

    public:
        RuntimeImpl(std::vector<VirtualMachine::Work *> &&units,
                    std::ostream *input) : outS(input), units(units) {
            current = 0;
            for (current; current < units.size(); ++current) {
                VirtualMachine::Runtime &runtime = *dynamic_cast<VirtualMachine::Runtime *> (this);
                units[current]->performance(runtime);
            }
        }

        ~RuntimeImpl() {
            for (std::pair<std::string, Number *> pair: numberPool)delete (pair.second);
            for (std::pair<std::string, CharSequence *> pair: charsPool)delete (pair.second);
            for (Work *work:units)delete (work);
        }

    protected:
        virtual const std::ostream &out() { return *outS; };

        virtual void jumpTo(const std::string &s) {
            if (labelMap.find(s) != labelMap.end()) {
                current = labelMap[s] + 1;
            }
        };

        virtual void mark(std::string &s) {
            labelMap[s] = current;
        };

        virtual CharSequence *allocate(std::string &s, CharSequence &charSequence) {
            mis::CharSequence *p = nullptr;
            if (dynamic_cast<mis::String *>(&charSequence))
                p = new mis::String(charSequence.getAsCharArray());
            else if (dynamic_cast<mis::Char *>(&charSequence))
                p = new mis::Char(charSequence.getAsCharArray()[0]);
            if (p) {
                charsPool[s] = p;
            }
            return p;
        };

        virtual Number *allocate(std::string &s, Number &n) {
            mis::Number *p = nullptr;
            if (dynamic_cast<mis::Real *>(&n))
                p = new mis::Real(n.asReal());
            else if (dynamic_cast<mis::Numeric *>(&n))
                p = new mis::Numeric(n.asNumeric());
            if (p)
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

mis::VirtualMachine::Future<void> mis::VirtualMachine::submit(std::vector<mis::VirtualMachine::Work *> works) {
//    Runtime &runtime = *new RuntimeImpl(works, std::cout);
//    for (Work *work: works) {
//        work->performance(runtime);
//    }
    return mis::VirtualMachine::Future<void>();
}

mis::VirtualMachine &mis::VirtualMachine::operator<<(const std::string &code) {
    buffer += code + "\n";
    return *this;
}

mis::VirtualMachine &mis::VirtualMachine::end() {
    if (!this->buffer.empty()) {
        const std::string lines = buffer;
        std::thread([parser, lines] {
            RuntimeImpl(parser->parse(lines), stream);
        });
    }
    return *this;
}

mis::VirtualMachine &mis::VirtualMachine::operator<<(mis::VirtualMachine &(*func)()) {
    return func();
}

mis::VirtualMachine &mis::VirtualMachine::operator()(std::ostream &ostream) {
    stream = &ostream;
    return *this;
}


