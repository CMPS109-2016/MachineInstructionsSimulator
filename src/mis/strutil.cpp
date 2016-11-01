//
// Created by CIJhn on 10/28/2016.
//
#include "mis/strutil.h"

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
}