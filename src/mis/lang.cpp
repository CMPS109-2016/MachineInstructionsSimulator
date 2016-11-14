//
// Created by CIJhn on 10/29/2016.
//

#include "mis/lang.h"

namespace mis {
    static inline Number &&performance(Number &a, Number &b, auto op);

    static auto ADD = [](auto &&a, auto &&b) { return a + b; };
    static auto SUB = [](auto &&a, auto &&b) { return a - b; };
    static auto MUL = [](auto &&a, auto &&b) { return a * b; };
    static auto DIV = [](auto &&a, auto &&b) { return a * b; };

    const char *String::getAsCharArray() {
        return string.data();
    }

    String::String(char *s) {
        this->string = *new std::string(s);
    }

    String::String(const char *s) {
        this->string = *new std::string(s);
    }

    std::string &&String::to_string() {
        return std::move(std::string(string));
    }

    int String::length() {
        return (int) string.length();
    }

    String::~String() {
    }

    char String::getCharAt(int idx) {
        return string.at(idx);
    }

    void String::setCharAt(int idx, char c) {
        this->string.assign(idx, c);
    };

    const char *Char::getAsCharArray() {
        return &c;
    }

    Char::Char(char c) {
        this->c = c;
    }

    std::string &&Char::to_string() {
        return std::move(std::string(&c));
    }

    char Char::getChar() {
        return c;
    }

    int Char::length() {
        return 1;
    };

    double Number::asReal() { return data.d; }

    long Number::asNumeric() { return data.l; }

    bool Number::isNumeric() { return type; }

    bool Number::isReal() { return !isNumeric(); }

    Number::Number(long l) : type(true) {
        data.l = l;
    }

    Number::Number(double d) : type(false) {
        data.d = d;
    }

    Number::Number(Number &n) : data(n.data), type(n.type) {}

    Number::Number(Number &&n) : data(n.data), type(n.type) {}

    Number &&Number::operator+(Number &n) { return performance(n, *this, ADD); }

    Number &&Number::operator+(Number &&n) { return performance(n, *this, ADD); }

    Number &Number::operator+=(Number &n) {
        _assign(n, ADD);
        return *this;
    }

    Number &Number::operator+=(Number &&n) {
        _assign(n, ADD);
        return *this;
    }

    Number &&Number::operator*(Number &n) { return performance(n, *this, MUL); }

    Number &&Number::operator*(Number &&n) { return performance(n, *this, MUL); }

    Number &Number::operator*=(Number &n) {
        _assign(n, MUL);
        return *this;
    }


    Number &Number::operator*=(Number &&n) {
        _assign(n, MUL);
        return *this;
    }

    Number &&Number::operator-(Number &n) {
        return performance(*this, n, SUB);
    }

    Number &&Number::operator-(Number &&n) {
        return performance(*this, n, SUB);
    }

    Number &Number::operator-=(Number &n) {
        _assign(n, SUB);
        return *this;
    }

    Number &Number::operator-=(Number &&n) {
        _assign(n, SUB);
        return *this;
    }

    Number &&Number::operator/(Number &n) {
        return performance(*this, n, DIV);
    }

    Number &&Number::operator/(Number &&n) {
        return performance(*this, n, DIV);
    }

    Number &Number::operator/=(Number &n) {
        _assign(n, DIV);
        return *this;
    }

    Number &Number::operator/=(Number &&n) {
        _assign(n, DIV);
        return *this;
    }

    bool Number::operator==(long l) {
        return this->isNumeric() ? data.l == l : data.d == l;
    }

    bool Number::operator==(double d) {
        return this->isNumeric() ? data.l == d : data.d == d;
    }

    void Number::_assign(Number &n, auto op) {
        if (this->isNumeric())
            if (n.isNumeric())
                this->data.l = op(data.l, n.asNumeric());
            else
                this->data.d = op(data.l, n.asReal());
        else if (n.isNumeric())
            data.d = op(data.d, n.asNumeric());
        else data.d *= op(data.d, n.asReal());
    }

    std::string &&Number::to_string() {
        return std::move(isNumeric() ? std::to_string(data.l) : std::to_string(data.d));
    }

    Number &Number::operator=(Number &n) {
        this->data = n.data;
        this->type = n.type;
        return *this;
    }

    Number &Number::operator=(Number &&n) {
        this->data = n.data;
        this->type = n.type;
        return *this;
    }

    bool Number::operator==(int i) {
        return this->isNumeric() ? data.l == i : data.d == i;
    }

    static inline Number &&performance(Number &a, Number &b, auto op) {
        if (a.isNumeric())
            if (b.isNumeric())
                return std::move(Number(op(a.asNumeric(), b.asNumeric())));
            else return std::move(Number(op(a.asNumeric(), b.asReal())));
        else if (b.isNumeric())
            return std::move(Number(op(a.asReal(), b.asNumeric())));
        else return std::move(Number(op(a.asReal(), b.asReal())));
    }

    bool Number::operator!=(int i) {
        return !this->operator==(i);
    }

    bool Number::operator!=(long l) {
        return !this->operator==(l);
    }

    bool Number::operator!=(double d) {
        return !this->operator==(d);
    }

    bool Number::operator>(Number &n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l > n.data.l : data.l > n.data.d :
               n.isNumeric() ? data.d > n.data.l : data.d > n.data.d;
    }

    bool Number::operator>(Number &&n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l > n.data.l : data.l > n.data.d :
               n.isNumeric() ? data.d > n.data.l : data.d > n.data.d;
    }

    bool Number::operator>=(Number &n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l >= n.data.l : data.l >= n.data.d :
               n.isNumeric() ? data.d >= n.data.l : data.d >= n.data.d;
    }

    bool Number::operator>=(Number &&n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l >= n.data.l : data.l >= n.data.d :
               n.isNumeric() ? data.d >= n.data.l : data.d >= n.data.d;
    }

    bool Number::operator<(Number &n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l < n.data.l : data.l < n.data.d :
               n.isNumeric() ? data.d < n.data.l : data.d < n.data.d;
    }

    bool Number::operator<(Number &&n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l < n.data.l : data.l < n.data.d :
               n.isNumeric() ? data.d < n.data.l : data.d < n.data.d;
    }

    bool Number::operator<=(Number &n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l <= n.data.l : data.l <= n.data.d :
               n.isNumeric() ? data.d <= n.data.l : data.d <= n.data.d;
    }

    bool Number::operator<=(Number &&n) {
        return this->isNumeric() ?
               n.isNumeric() ? data.l <= n.data.l : data.l <= n.data.d :
               n.isNumeric() ? data.d <= n.data.l : data.d <= n.data.d;
    }
}