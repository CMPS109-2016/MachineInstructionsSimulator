//
// Created by CIJhn on 10/29/2016.
//

#include "mis/lang.h"

namespace mis {

    Numeric::Numeric(long v) : l(v) {};

    double Numeric::asReal() {
        return (double) l;
    };

    long Numeric::asNumeric() {
        return l;
    };

    Real::Real(double v) : d(v) {};

    double Real::asReal() {
        return d;
    };

    long Real::asNumeric() {
        return (long) d;
    };


    const char *String::getAsCharArray() {
        return string.data();
    }

    String::String(const char *s) {
        this->string = *new std::string(s);
    };

    const char *Char::getAsCharArray() {
        return &c;
    }

    Char::Char(char c) {
        this->c = c;
    };

    template<typename T>
    bool Optional<T>::isPresent() {
        return value != nullptr;
    }

    template<typename T>
    T *Optional<T>::get() {
        return value;
    }

    template<typename T>
    void Optional<T>::ifPresent(void (*func)(T *)) {
        if (isPresent())
            func(this->value);
    }

    template<typename T>
    Optional<T>::Optional(T *v) :value(v) {
    }


    template<typename T>
    Optional<T> Optional<T>::empty() {
        return Optional<T>(nullptr);
    }

    template<typename T>
    Optional<T> Optional<T>::nullable(T *v) {
        return Optional<T>(v);
    }
}