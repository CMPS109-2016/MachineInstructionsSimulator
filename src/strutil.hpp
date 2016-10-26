//
// Created by CIJhn on 10/24/2016.
//

#ifndef __MISSTRUTLS__
#define __MISSTRUTLS__

#include <string>
#include <vector>

namespace mis {
    void split(const std::string &s, const std::string &delim, std::vector<std::string> &vector) {
        size_t last = 0;
        size_t index = s.find_first_of(delim, last);
        while (index != std::string::npos) {
            vector.push_back(s.substr(last, index - last));
            last = index + 1;
            index = s.find_first_of(delim, last);
        }
        if (index - last > 0)
            vector.push_back(s.substr(last, index - last));
    }
}

#endif