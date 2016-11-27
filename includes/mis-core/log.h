//
// Created by CIJhn on 11/26/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_LOG_H
#define MACHINEINSTRUCTIONSSIMULATOR_LOG_H


namespace mis {
    void _lgcapture(const std::string &&s, const std::string &clz, const std::string &func, int line);

#  define log(s) _lgcapture(s,__FILE__,__FUNCTION__,__LINE__)
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_LOG_H
