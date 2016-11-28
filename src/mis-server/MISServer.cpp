//
// Created by CIJhn on 11/27/2016.
//

#include <thread>
#include "mis-instructions/default_instructions.h"
#include "mis-core/bit.h"
#include "mis-server/MISServer.h"

namespace mis {
    bool MISServer::isTerminated() {
        return terminate;
    }

    void MISServer::terminated() {
        terminate = true;
        mutex.lock();
        delete (socket);
        delete (virtualMachine);
        delete (parser);
        for (auto work:workingQueue)
            delete (work);
        mutex.unlock();
    }

    void MISServer::start() {
        startingLock.lock();
        std::thread([this]() {
            while (!terminate) {
                TCPSocket *tcp = socket->getConnection();
                Record *rec = new Record(tcp->getRemoteAddress(), std::chrono::system_clock::now());
                Worker *worker = new Worker(tcp, parser, virtualMachine, [this](Worker *w) {
                    this->garbage(w);
                }, rec);
                workingQueue.push_back(worker);
                history.push_back(rec);
                worker->start();
            }
            startingLock.unlock();
        });
    }

    void MISServer::garbage(MISServer::Worker *worker) {
        if (worker == nullptr)
            return;
        mutex.lock();
        auto itr = workingQueue.begin();
        for (; itr != workingQueue.end(); itr++) {
            if ((*itr) == worker)
                break;
        }
        if (itr != workingQueue.end())
            workingQueue.erase(itr);
        delete (worker);
        mutex.unlock();
    }

    MISServer::~MISServer() {
        for (Worker *worker: workingQueue)
            delete (worker);
        for (Record *his:history)
            delete (his);
    }

    MISServer::MISServer(TCPServerSocket *socket, VirtualMachine *virtualMachine, Parser *parser) : socket(socket),
                                                                                                    virtualMachine(
                                                                                                            virtualMachine),
                                                                                                    parser(parser) {}

    const vector<MISServer::Record *> &MISServer::getHistory() const {
        return history;
    }

    void MISServer::Worker::start() {
        std::thread([this]() {
            char lengthBuffer[4];
            int read = socket->readFromSocketWithTimeout(lengthBuffer, 4, 10, 0);
            if (read != 4) {
                callback(this);
                return;
            }
            int length = mis::readInt(lengthBuffer);
            char buffer[length];
            read = socket->readFromSocketWithTimeout(buffer, length, 60, 0);
            if (read != length) {
                callback(this);
                return;
            }
            std::string content(buffer);
            std::vector<VirtualMachine::Work *> works = parser->parse(content);

            std::stringstream resultStream;
            virtualMachine->run(works, &resultStream);
            std::string result;
            resultStream >> result;
            memset(lengthBuffer, 0, 4);
            writeInt(lengthBuffer, (int) result.length());
            socket->writeToSocket(lengthBuffer, 4);
            socket->writeToSocket(result.c_str(), result.length());

            this->record->setDuration(std::chrono::system_clock::now());
            callback(this);
        });
    }

    MISServer::Worker::~Worker() {
        delete (socket);
    }


    MISServer::Worker::Worker(TCPSocket *socket, Parser *parser, VirtualMachine *virtualMachine,
                              const function<void(MISServer::Worker *)> &callback, MISServer::Record *record) : socket(
            socket), parser(parser), virtualMachine(virtualMachine), callback(callback), record(record) {}

    std::string MISServer::Record::getStartTime() const {
        auto t = std::chrono::system_clock::to_time_t(startTime);
        tm *local = localtime(&t);
        return std::string(std::to_string(local->tm_year) +
                           "-" +
                           std::to_string(local->tm_mon) +
                           "-" +
                           std::to_string(local->tm_mday) +
                           " " +
                           std::to_string(local->tm_hour) +
                           ":" +
                           std::to_string(local->tm_min) +
                           ":" +
                           std::to_string(local->tm_sec)
        );
    }

    const string &MISServer::Record::getIp() const {
        return ip;
    }

    std::string MISServer::Record::getDuration() const {
        return std::to_string(duration);
    }

    MISServer::Record::Record(const string &ip, const chrono::time_point &startTime) : ip(ip), startTime(startTime) {}

    void MISServer::Record::setDuration(const chrono::time_point &end) {
        auto diff = startTime - end;
        duration = diff.count();
    }
}
