//
// Created by CIJhn on 10/29/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_LANG_H
#define MACHINEINSTRUCTIONSSIMULATOR_LANG_H

#include <string>
#include <functional>
#include "mis-core/log.h"

namespace mis {
    /**
     * An easy super-class for all the mis base class that need to be printed.
     */
    struct Base {

        /**
         * Transfer this instance to string.
         * */
        virtual std::string to_string()=0;
    };

    /**
     * Box the basic number calculation boxing.
     * */
    struct Number : public virtual Base {
    public:
        /**
         * Cast this number to a real.
         * */
        double asReal();

        /**
        * Cast this number to a mimeric.
        * */
        long asNumeric();

        /**
         * If this number is a real.
         * */
        bool isReal();

        /**
        * If this number is a mimeric.
        * */
        bool isNumeric();

        /**********************
         * Common constructors
         **********************/

        Number(long);

        Number(double);

        Number(Number &);

        Number(Number &&);

        /**********************
         * Common operators
         **********************/

        virtual Number operator+(Number &n);

        virtual Number operator+(Number &&n);

        virtual Number &operator+=(Number &n);

        virtual Number &operator+=(Number &&n);

        virtual Number operator-(Number &n);

        virtual Number operator-(Number &&n);

        virtual Number &operator-=(Number &n);

        virtual Number &operator-=(Number &&n);

        virtual Number operator*(Number &n);

        virtual Number operator*(Number &&n);

        virtual Number &operator*=(Number &n);

        virtual Number &operator*=(Number &&n);

        virtual Number operator/(Number &n);

        virtual Number operator/(Number &&n);

        virtual Number &operator/=(Number &n);

        virtual Number &operator/=(Number &&n);

        virtual bool operator==(long);

        virtual bool operator==(int);

        virtual bool operator==(double);

        virtual bool operator!=(int);

        virtual bool operator!=(long);

        virtual bool operator!=(double);

        virtual bool operator>(Number &n);

        virtual bool operator>(Number &&n);

        virtual bool operator>=(Number &n);

        virtual bool operator>=(Number &&n);

        virtual bool operator<(Number &n);

        virtual bool operator<(Number &&n);

        virtual bool operator<=(Number &n);

        virtual bool operator<=(Number &&n);

        virtual Number &operator=(Number &n);

        virtual Number &operator=(Number &&n);

        virtual std::string to_string() override;

    private:
        union { long l; double d; } data;
        bool type;

        inline void _assign(Number &n, auto);
    };

    template<typename T>
    using Supplier = std::function<T(void)>;

    struct CharSequence : public virtual Base {
        virtual const char *getAsCharArray()=0;

        /**
         * The length of this sequence.
         * */
        virtual int length()=0;

        virtual std::string to_string() =0;
    };

    class String : public CharSequence {
    private:
        std::string string;
    public:
        String(std::string &&s);

        String(const std::string &s);

        String(const char *s);

        String(char *s);

        ~String();

        /**
         * Set char at a certain pos
         * */
        virtual void setCharAt(int idx, char c);

        /** Get char at cuertain pos
         * */
        virtual char getCharAt(int idx);

        virtual const char *getAsCharArray() override;

        virtual std::string to_string() override;

        virtual int length() override;
    };

    class Char : public CharSequence {
    private:
        char c;
    public:
        Char(char c);

        virtual char getChar();

        virtual const char *getAsCharArray() override;

        virtual std::string to_string() override;

        virtual int length() override;
    };

    struct Terminable {
        virtual void terminate()=0;

        virtual bool isTerminated()=0;
    };

    /**
     * A short-cut to determine the type.
     * */
    template<typename Base, typename T>
    inline bool instanceof(T *ptr) {
        return dynamic_cast<Base *>(ptr) != nullptr;
    }

    template<class T>
    struct typeref {};

    template<class T, T value>
    struct valueref {};
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_LANG_H
