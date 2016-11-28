//
// Created by CIJhn on 11/27/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_BIT_H
#define MACHINEINSTRUCTIONSSIMULATOR_BIT_H

#include <cstdlib>

namespace mis {
    /**
     * Read template type to array.
     * */
    template<typename T, size_t sz = sizeof(T)>
    T read(char cha[sz]);

    /**
     * Write template type to array.
     * */
    template<typename T, size_t sz = sizeof(T)>
    void write(char cha[sz], T v);

    /**
     * Read an integer from a char array.
     * */
    int readInt(char *cha);

    /**
     * Write an integer to an array.
     * */
    void writeInt(char cha[4], int i);
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_BIT_H
