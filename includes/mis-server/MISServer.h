//
// Created by CIJhn on 11/27/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H
#define MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H

#include <atomic>
#include <mutex>
#include "TCPServerSocket.h"
#include "mis-core/VirtualMachine.h"
#include "mis-core/parser.h";

namespace mis {
    struct MISServer {
        bool isTerminated();

        void terminated();

        void start();

        MISServer(TCPServerSocket *socket, VirtualMachine *virtualMachine, Parser *parser);

        virtual ~MISServer();

        static MISServer *createDefaultServer();

    private:
        TCPServerSocket *socket;
        std::atomic<bool> terminate;
        std::mutex mutex, startingLock;
        VirtualMachine *virtualMachine;
        Parser *parser;

        std::vector<Worker *> workingQueue;

        void garbage(Worker *worker);

        class Worker {
            TCPSocket *socket;
            Parser *parser;
            VirtualMachine *virtualMachine;
            std::function<void(Worker *)> callback;

        public:
            Worker(TCPSocket *socket, Parser *parser, VirtualMachine *virtualMachine,
                   const function<void(Worker *)> &callback);

            void start();

            virtual ~Worker();
        };
    };
}


#endif //MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H
