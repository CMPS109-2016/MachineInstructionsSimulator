#include "process.h"

#include "tokens"
#include "strutil.h"

class Process {
    void mark();
};


class Command {
    virtual void performance(Process &process)=0;
};

class Label {
    const std::string &name;
public:
    Label(const std::string &name) : name(name) {
    }

    const std::string &getName() {
        return name;
    }
};

class CmdDeclVar : public Command {
private:
    Var &var;

public:
    virtual void performance(Process &process) {
        process.request(var);
    }
};

class CmdSub : public Command {
    Var &target;
    Value &a, &b;
public:
    virtual void performance(Process &process) {
        process.update(target, process.get(a) - process.get(b));
    }
};

class CmdAssign : public Command {
    Var &target;
    Value &src;
public:
    virtual void performance(Process &process) {
        process.update(target, process.get(src));
    }
};

class CmdLabel : public Command {
    Label &label;
public:
    virtual void performance(Process &process) {
        process.mark(label);
    }
};

class CmdJmp : public Command {
    Label &label;
public:
    virtual void performance(Process &process) {
        process.jumpTo(label);
    }
};

class CmdJmpz : public Command {
    Label &label;
    Value &val;
    bool mask;
public:
    virtual void performance(Process &process) {
        if (process.get(val) ^ mask)
            process.jumpTo(label);
    }
};

class CmdSleep : public Command {
    Value &val;

public:
    virtual void performance(Process &process) {
        std::this_thread::sleep_for(process.get(val));
    }
};

class CmdAdd : public Command {
    Var &target;
    std::vector<Value &> vals;
public:
    virtual void performance(Process &process) {
        auto sum = 0;
        for (Value &v: vals)
            sum += process.get(v);
        process.update(target, sum);
    }
};