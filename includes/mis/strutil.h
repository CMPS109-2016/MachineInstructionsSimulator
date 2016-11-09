//
// Created by CIJhn on 10/24/2016.
//

#ifndef __MISSTRUTLS__
#define __MISSTRUTLS__

#include <string>
#include <vector>
#include <array>

namespace mis {
    std::vector<std::string> &split(const std::string &s, const std::string &delim, std::vector<std::string> &vector);

    void trim(std::string &s);

    std::vector<std::string> &splitDetectStringChar(const std::string &s, char delim, std::vector<std::string> &vector);

    int get_type(std::string &s);
}

#endif