//
// Created by CIJhn on 10/29/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_LANG_H
#define MACHINEINSTRUCTIONSSIMULATOR_LANG_H

#include <string>
#include <functional>

namespace mis {
    struct Base {
        virtual std::string &&to_string()=0;
    };

    struct Number : public virtual Base {
    public:
        double asReal();

        long asNumeric();

        bool isReal();

        bool isNumeric();

        Number(long);

        Number(double);

        Number(Number &);

        Number(Number &&);

        virtual Number &&operator+(Number &n);

        virtual Number &&operator+(Number &&n);

        virtual Number &operator+=(Number &n);

        virtual Number &operator+=(Number &&n);

        virtual Number &&operator-(Number &n);

        virtual Number &&operator-(Number &&n);

        virtual Number &operator-=(Number &n);

        virtual Number &operator-=(Number &&n);

        virtual Number &&operator*(Number &n);

        virtual Number &&operator*(Number &&n);

        virtual Number &operator*=(Number &n);

        virtual Number &operator*=(Number &&n);

        virtual Number &&operator/(Number &n);

        virtual Number &&operator/(Number &&n);

        virtual Number &operator/=(Number &n);

        virtual Number &operator/=(Number &&n);

        virtual bool operator==(long);

        virtual bool operator==(int);

        virtual bool operator==(double);

        virtual Number &operator=(Number &n);

        virtual Number &operator=(Number &&n);


        virtual std::string &&to_string() override;

    private:
        union { long l; double d; } data;
        bool type;

        inline void _assign(Number &n, auto);
    };

    template<typename T>
    using Supplier = std::function<T(void)>;

    struct CharSequence : public virtual Base {
        virtual const char *getAsCharArray()=0;

        virtual int length()=0;
    };

    class String : public CharSequence {
    private:
        std::string string;
    public:
        String(const char *s);

        String(char *s);

        ~String();

        virtual void setCharAt(int idx, char c);

        virtual char getCharAt(int idx);

        virtual const char *getAsCharArray() override;

        virtual std::string &&to_string() override;

        virtual int length() override;
    };

    class Char : public CharSequence {
    private:
        char c;
    public:
        Char(char c);

        virtual char getChar();

        virtual const char *getAsCharArray() override;

        virtual std::string &&to_string() override;

        virtual int length() override;
    };

    struct Terminable {
        virtual void terminate()=0;

        virtual bool isTerminated()=0;
    };

    template<typename T>
    struct Predicate {
        virtual bool accept(T t) const =0;
    };

    template<class T>
    struct typeref {};

    template<class T, T value>
    struct valueref {};
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_LANG_H
