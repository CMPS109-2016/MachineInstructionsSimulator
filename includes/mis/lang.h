//
// Created by CIJhn on 10/29/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_LANG_H
#define MACHINEINSTRUCTIONSSIMULATOR_LANG_H

#include <string>

namespace mis {
    struct Number {
        virtual double asReal()=0;

        virtual long asNumeric()=0;
    };

    class Numeric : public Number {
    private:
        long l;
    public:
        Numeric(long);

        virtual double asReal() override;

        virtual long asNumeric() override;
    };

    class Real : public Number {
    private:
        double d;
    public:
        Real(double);

        virtual double asReal() override;

        virtual long asNumeric() override;
    };

    struct CharSequence {
        virtual const char *getAsCharArray()=0;
    };

    class String : public CharSequence {
    private:
        std::string string;
    public:
        String(const char *s);

        virtual const char *getAsCharArray() override;
    };

    class Char : public CharSequence {
    private:
        char c;
    public:
        Char(char c);

        virtual const char *getAsCharArray() override;
    };

    template<typename T>
    class Optional {
    private:
        T *value;

        Optional(T *v);

    public:
        static Optional<T> empty();

        static Optional<T> nullable(T *v);

        bool isPresent();

        void ifPresent(void (*)(T *value));

        T *get();
    };

    template<class T>
    struct typeref {};

    template<class T, T value>
    struct valueref {};
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_LANG_H
