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
        delete (socket);
        delete (virtualMachine);
        delete (parser);
    }

    void MISServer::start() {
        startingLock.lock();
        std::thread([]() {
            while (!terminate) {
                TCPSocket *tcp = socket->getConnection();
                Worker *worker = new Worker(tcp, parser, virtualMachine, [this](Worker *w) {
                    this->garbage(w);
                });
                worker->start();
            }
            startingLock.unlock();
        });
    }

    void MISServer::garbage(MISServer::Worker *worker) {
        mutex.lock();
        auto itr = workingQueue.begin();
        for (; itr != workingQueue.end(); itr++) {
            if ((*itr) == worker)
                break;
        }
        workingQueue.erase(itr);
        delete (worker);
        mutex.unlock();
    }

    MISServer::~MISServer() {
        for (Worker *worker:workingQueue) {
            delete (worker);
        }
    }

    MISServer::MISServer(TCPServerSocket *socket, VirtualMachine *virtualMachine, Parser *parser) : socket(socket),
                                                                                                    virtualMachine(
                                                                                                            virtualMachine),
                                                                                                    parser(parser) {}

    MISServer *MISServer::createDefaultServer() {
        TCPServerSocket *socket = new TCPServerSocket("127.0.0.1", 450, 16);
        Parser::Builder builder;
        register_default(builder);
        return new MISServer(socket, new VirtualMachine(), builder.build());
    }


    void MISServer::Worker::start() {
        std::thread([socket, callback, virtualMachine, parser]() {
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

            callback(this);
        });
    }

    MISServer::Worker::~Worker() {
        delete (socket);
    }


    MISServer::Worker::Worker(TCPSocket *socket, Parser *parser, VirtualMachine *virtualMachine,
                              const function<void(MISServer::Worker *)> &callback) : socket(socket), parser(parser),
                                                                                     virtualMachine(virtualMachine),
                                                                                     callback(callback) {}

}
