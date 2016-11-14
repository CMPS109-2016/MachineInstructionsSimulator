//
// Created by CIJhn on 10/28/2016.
//
#include "mis/strutil.h"
#include <fstream>
#include <iostream>

namespace mis {
//    template<std::size_t size>
//    std::array<std::string, size>
//    split(const std::string &s, const std::string &delim, std::array<std::string, size> &arr) {
//        size_t last = 0;
//        size_t index = s.find_first_of(delim, last);
//        int idx = 0;
//        while (index != std::string::npos) {
//            arr[idx] = s.substr(last, index - last);
//            last = index + 1;
//            index = s.find_first_of(delim, last);
//            ++idx;
//        }
//        if (index - last > 0)
//            arr.push_back(s.substr(last, index - last));
//        return arr;
//    };

    std::vector<std::string> &split(const std::string &s, const std::string &delim, std::vector<std::string> &vector) {
        size_t last = 0;
        size_t index = s.find_first_of(delim, last);
        while (index != std::string::npos) {
            vector.push_back(s.substr(last, index - last));
            last = index + 1;
            index = s.find_first_of(delim, last);
        }
        if (index - last > 0)
            vector.push_back(s.substr(last, index - last));
        return vector;
    }

    int get_type(std::string &s) {
        bool hasPoint = false;
        for (int i = 0; i < s.length(); ++i) {
            char c = s[i];
            if (c < '0' || c > '9') {
                if (c == '.' && !hasPoint)
                    hasPoint = true;
                else
                    return -1;
            }
        }
        return hasPoint ? 1 : 0;
    }

    void trim(std::string &s) {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(0, s.find_first_not_of("\t"));
        s.erase(s.find_last_not_of(" ") + 1);
        s.erase(s.find_last_not_of("\r") + 1);
        s.erase(s.find_last_not_of("\n") + 1);
        s.erase(s.find_last_not_of("\t") + 1);
    }


    std::vector<std::string> &
    splitDetectStringChar(const std::string &s, char delim, std::vector<std::string> &vector) {
        std::string temp = s;
        std::string::size_type last = 0;
        int embracedState = 0;
        for (std::string::size_type current = 0; current < s.length(); ++current) {
            char c = temp[current];
            if (c == delim && embracedState == 0) {
                vector.push_back(std::string(temp, last, current - last));
                last = current + 1;
            } else if (c == '\'') {
                if (embracedState == 0)
                    embracedState = -1;
                else if (embracedState == -1)
                    embracedState = 0;
                else {
                    //syntax error: "xxxx'
                    throw std::bad_exception();
                }
            } else if (c == '\"') {
                if (embracedState == 0)
                    embracedState = 1;
                else if (embracedState == 1)
                    embracedState = 0;
                else {
                    //syntax error: 'xxxx"
                    throw std::bad_exception();
                }
            }
        }

        if (embracedState != 0) {
            throw std::bad_exception();
        }
        if (last != s.length())
            vector.push_back(std::string(temp, last, s.length() - last));
        return vector;
    }

    std::vector<std::string> &readFileByLines(std::string &filename, std::vector<std::string> &vector, int bufferSize) {
        char buff[bufferSize];
        std::ifstream ifstream(filename);
        while (!ifstream.eof()) {
            ifstream.getline(buff, bufferSize);
            vector.push_back(buff);
        }
        return vector;
    }

    std::vector<std::string> &
    readFileByLines(std::string &&filename, std::vector<std::string> &vector, int bufferSize) {
        char buff[bufferSize];
        std::ifstream ifstream(filename);
        while (!ifstream.eof()) {
            ifstream.getline(buff, bufferSize);
            vector.push_back(buff);
        }
        return vector;
    }

    std::string readFileToString(std::string &filename) {
        std::ifstream ifstream(filename);
        std::string s = std::string(std::istreambuf_iterator<char>(ifstream),
                                    std::istreambuf_iterator<char>());
        ifstream.close();
        return s;
    }

    std::string readFileToString(std::string &&filename) {
        std::ifstream ifstream(filename);
        std::string s = std::string(std::istreambuf_iterator<char>(ifstream),
                                    std::istreambuf_iterator<char>());
        ifstream.close();
        return s;
    }
}