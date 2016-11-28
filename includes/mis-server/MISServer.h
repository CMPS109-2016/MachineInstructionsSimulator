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
        /**
         * The common record of a worker.
         * */
        struct Record {
            Record(const string &ip, const chrono::system_clock::time_point &startTime);

            /**The ip of the work*/
            const string &getIp() const;

            /**The duration of the work.*/
            std::string getDuration() const;

            /**The starting time of the work.*/
            std::string getStartTime() const;

            /**Set the duration from the end time*/
            void setDuration(const chrono::system_clock::time_point &end);

        private:
            std::string ip;
            std::chrono::system_clock::time_point startTime;
            long duration;
        };

        /**
         * If this server is terminated
         * */
        bool isTerminated();

        /**
         * Terminate the server.
         * */
        void terminated();

        /**
         * Start the server.
         * */
        void start();

        /**
         * @param socket lisnter socket will be used.
         * @param virtualMachine the virtualmachine handing the execution.
         * @param parser the parser handling the parsing.
         * */
        MISServer(TCPServerSocket *socket, VirtualMachine *virtualMachine, Parser *parser);

        virtual ~MISServer();

        /**
         * Get the history list.
         * */
        const vector<Record *> &getHistory() const;

    private:
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
