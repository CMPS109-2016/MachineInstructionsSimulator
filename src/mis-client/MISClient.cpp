//
// Created by CIJhn on 11/27/2016.
//

#include <queue>
#include <thread>
#include "mis-core/bit.h"
#include "mis-client/MISClient.h"

void mis::MISClient::queryCompile(const std::string &string, std::ostream *out) {
    Worker *worker = nullptr;
    char add[string.size()];
    memcpy(add, string.c_str(), string.size());
    TCPSocket *socket = new TCPSocket(add, 450, 65536, 65536);
    if (socket->isPeerDisconnected()) {
        std::cout << "Cannot connect to server, query fail." << std::endl;
        return;
    }
    worker = new Worker(string, out, socket, [this](Worker *w) {
        this->garbage(w);
    });
    mutex.lock();
    workingQueue.push_back(worker);
    mutex.unlock();
    worker->work();
}

void mis::MISClient::queryCompile(std::string &&string, std::ostream *out) {
    Worker *worker = nullptr;
    char add[string.size()];
    memcpy(add, string.c_str(), string.size());
    TCPSocket *socket = new TCPSocket(add, 450, 65536, 65536);
    if (socket->isPeerDisconnected()) {
        std::cout << "Cannot connect to server, query fail." << std::endl;
        return;
    }
    worker = new Worker(string, out, socket, [this](Worker *w) {
        this->garbage(w);
    });
    mutex.lock();
    workingQueue.push_back(worker);
    mutex.unlock();
    worker->work();
}

mis::MISClient::MISClient(const std::string &remote) : remoteAddress(remote) {
}

void mis::MISClient::garbage(mis::MISClient::Worker *worker) {
    mutex.lock();
    auto itr = workingQueue.begin();
    for (; itr != workingQueue.end(); itr++)
        if (*itr == worker)
            break;
    workingQueue.erase(itr);
    delete (worker);
    mutex.unlock();
}

mis::MISClient::~MISClient() {
    for (Worker *worker :workingQueue) {
        delete (worker);
    }
}

mis::MISClient::Worker::~Worker() {
    delete (socket);
}


void mis::MISClient::Worker::work() {
    std::thread([this]() {
        char lengthBuffer[4];
        mis::writeInt(lengthBuffer, (int) this->content.length());
        int wrote = socket->writeToSocket(lengthBuffer, 4);
        if (wrote == -1) {
            this->garbage(this);
            return;
        }
        while (wrote < 4) {
            socket->writeToSocket(lengthBuffer + wrote, 4 - wrote);
        }

        const char *content = this->content.c_str();
        wrote = socket->writeToSocket(content, this->content.size());
        if (wrote == -1) {
            this->garbage(this);
            return;
        }
        while (wrote < this->content.size()) {
            socket->writeToSocket(content + wrote, this->content.size() - wrote);
        }

        memset(lengthBuffer, 0, 4);
        int read = socket->readFromSocketWithTimeout(lengthBuffer, 4, 120, 0);
        if (read != 4) {
            this->garbage(this);
            return;
        }
        int length = mis::readInt(lengthBuffer);
        char buffer[length];
        read = socket->readFromSocketWithTimeout(buffer, length, 120, 0);
        if (read != 4) {
            this->garbage(this);
            return;
        }
        std::string result(buffer);
        *this->stream << result << std::endl;
        garbage(this);
    });
}

mis::MISClient::Worker::Worker(const string &content, ostream *stream, TCPSocket *socket,
                               const function<void(mis::MISClient::Worker *)> &garbage) : content(content),
                                                                                          stream(stream),
                                                                                          socket(socket),
                                                                                          garbage(garbage) {}

