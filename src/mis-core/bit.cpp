#include <cstddef>

//
// Created by CIJhn on 11/27/2016.
//
namespace mis {
    template<typename T, size_t sz = sizeof(T)>
    T read(char *cha) {
        T result;
        for (size_t i = sz - 1; i >= 0; i--)
            result |= cha[i] << i * 8;
        return result;
    }

    template<typename T, size_t sz = sizeof(T)>
    void write(char *cha, T v) {
        for (size_t i = sz - 1; i >= 0; i--)
            cha[i] = (v >> (i * 8)) & 0xFF;
    }

    int readInt(char *cha) {
        return cha[3] << 24 | cha[2] << 16 | cha[1] << 8 | cha[0];
    }

    void writeInt(char *cha, int v) {
        cha[3] = (char) ((v >> 24) & 0xff);
        cha[2] = (char) ((v >> 16) & 0xff);
        cha[1] = (char) ((v >> 8) & 0xff);
        cha[0] = (char) (v & 0xff);
    }
}
