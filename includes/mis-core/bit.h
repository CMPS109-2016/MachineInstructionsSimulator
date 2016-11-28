//
// Created by CIJhn on 11/27/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_BIT_H
#define MACHINEINSTRUCTIONSSIMULATOR_BIT_H

#include <cstdlib>

namespace mis {
    template<typename T, size_t sz = sizeof(T)>
    T read(char cha[sz]);

    template<typename T, size_t sz = sizeof(T)>
    void write(char cha[sz], T v);

    int readInt(char *cha);

    void writeInt(char cha[4], int i);
}
#endif //MACHINEINSTRUCTIONSSIMULATOR_BIT_H
