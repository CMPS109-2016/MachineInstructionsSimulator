//
// Created by CIJhn on 11/27/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H
#define MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H

#include <atomic>
#include <mutex>
#include <ctime>
#include <chrono>

#include "TCPServerSocket.h"
#include "mis-core/VirtualMachine.h"
#include "mis-core/parser.h"

namespace mis {
    struct MISServer {
        bool isTerminated();

        void terminated();

        void start();

        MISServer(TCPServerSocket *socket, VirtualMachine *virtualMachine, Parser *parser);

        virtual ~MISServer();

        const vector<Record *> &getHistory() const;

    private:
        struct Record {
            Record(const string &ip, const chrono::time_point &startTime);

            const string &getIp() const;

            std::string getDuration() const;

            std::string getStartTime() const;

            void setDuration(const chrono::time_point &end);

        private:
            std::string ip;
            std::chrono::time_point startTime;
            long duration;
        };

        class Worker {
            TCPSocket *socket;
            Parser *parser;
            VirtualMachine *virtualMachine;
            std::function<void(Worker *)> callback;
            Record *record;

        public:
            Worker(TCPSocket *socket, Parser *parser, VirtualMachine *virtualMachine,
                   const function<void(Worker *)> &callback, Record *record);

            void start();

            virtual ~Worker();
        };


        TCPServerSocket *socket;
        std::atomic<bool> terminate;
        std::mutex mutex, startingLock;
        VirtualMachine *virtualMachine;
        Parser *parser;

        std::vector<Worker *> workingQueue;
        std::vector<Record *> history;

        void garbage(Worker *worker);
    };
}


#endif //MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H
