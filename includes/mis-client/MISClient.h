//
// Created by CIJhn on 11/27/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_MISCLIENT_H
#define MACHINEINSTRUCTIONSSIMULATOR_MISCLIENT_H

#include <functional>
#include <mutex>
#include "TCPSocket.h"

namespace mis {
    struct MISClient {
        /**Require to compile a block of code.
         *
         * @param string the codes.
         * @param out the out strema of this compilation.
         * @param block if this compilation is blocking.
         * @return flase means this compilation failed.
         * */
        bool queryCompile(const std::string &string, std::ostream *out, bool block);

        /**Require to compile a block of code.
         *
         * @param string the codes.
         * @param out the out strema of this compilation.
         * @param block if this compilation is blocking.
         * @return flase means this compilation failed.
         * */
        bool queryCompile(std::string &&str, std::ostream *out, bool block);

        /**
         * @param remote The server ip.
         * */
        MISClient(const std::string &remote);

        virtual ~MISClient();

    private:
        struct Worker {
            void work(bool blocked);

            Worker(const string &content, ostream *stream, TCPSocket *socket, const function<void(Worker *)> &garbage);

            virtual ~Worker();

            std::string content;
            std::ostream *stream;
            TCPSocket *socket;//prepare to reused
            std::function<void(Worker *)> garbage;
        };

        std::string remoteAddress;
        std::vector<Worker *> workingQueue;

        std::mutex mutex;

        void garbage(Worker *worker);
    };
}


#endif //MACHINEINSTRUCTIONSSIMULATOR_MISCLIENT_H
