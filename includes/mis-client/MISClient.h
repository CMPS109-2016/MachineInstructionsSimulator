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
        void queryCompile(const std::string &string, std::ostream &out = std::cout);

        void queryCompile(std::string &&str, std::ostream &out = std::cout);

        MISClient(const std::string &remote);

        virtual ~MISClient();

    private:
        struct Worker {
            Worker(TCPSocket *socket);

            void work(const std::string &work, std::ostream &out);

            Worker(TCPSocket *socket, const function<void(Worker *)> &callback);

            virtual ~Worker();

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
