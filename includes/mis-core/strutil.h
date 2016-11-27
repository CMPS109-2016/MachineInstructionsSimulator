//
// Created by CIJhn on 10/24/2016.
//

#ifndef __MISSTRUTLS__
#define __MISSTRUTLS__

#include <string>
#include <vector>
#include <array>

namespace mis {

    /**
     * Spilt the string directy by the char. Stored the data into the vector.
     * */
    std::vector<std::string> &split(const std::string &s, const std::string &delim, std::vector<std::string> &vector);

    /**
     * Trim the string.
     * */
    void trim(std::string &s);

    /**
     * Split the string by the char, ignoring the case inside of " and '.
     * */
    std::vector<std::string> &splitDetectStringChar(const std::string &s, char delim, std::vector<std::string> &vector);

    /**
     * The helper method to inspect the stirng type, whether it's numeric(return 0)or real(return 1).
     * If it's just not a number, return -1
     * */
    int get_type(std::string &s);

    std::vector<std::string> &
    readFileByLines(std::string &filename, std::vector<std::string> &vector, int bufferSize = 256);

    std::vector<std::string> &
    readFileByLines(std::string &&filename, std::vector<std::string> &vector, int bufferSize = 256);

    std::string readFileToString(std::string &filename);

    std::string readFileToString(std::string &&filename);
}

#endif