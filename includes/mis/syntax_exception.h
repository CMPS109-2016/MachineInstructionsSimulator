//
// Created by CIJhn on 11/13/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_SYNTEX_EXCEPTION_H
#define MACHINEINSTRUCTIONSSIMULATOR_SYNTEX_EXCEPTION_H


#include <exception>
#include <string>

namespace mis {
    class syntax_exception : public virtual std::exception {
    public:
        syntax_exception(const std::string &error);

        const std::string &getError() const;
    private:
        std::string error;
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_SYNTEX_EXCEPTION_H
