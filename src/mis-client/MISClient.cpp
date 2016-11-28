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
    worker = new Worker(socket, [this](Worker *w) {
        this->garbage(w);
    });
    mutex.lock();
    workingQueue.push_back(worker);
    mutex.unlock();
    worker->work(string, out);
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
    worker = new Worker(socket, [this](Worker *w) {
        this->garbage(w);
    });
    mutex.lock();
    workingQueue.push_back(worker);
    mutex.unlock();
    worker->work(string, out);
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

mis::MISClient::Worker::Worker(TCPSocket *socket) : socket(socket) {
}

void mis::MISClient::Worker::work(const std::string &work, std::ostream *outstream) {
    std::function<void()> func(
            [this, work, outstream]() {
                char lengthBuffer[4];
                mis::writeInt(lengthBuffer, (int) work.length());
                int wrote = socket->writeToSocket(lengthBuffer, 4);
                if (wrote == -1) {
                    this->garbage(this);
                    return;
                }
                while (wrote < 4) {
                    socket->writeToSocket(lengthBuffer + wrote, 4 - wrote);
                }

                const char *content = work.c_str();
                wrote = socket->writeToSocket(content, work.size());
                if (wrote == -1) {
                    this->garbage(this);
                    return;
                }
                while (wrote < work.size()) {
                    socket->writeToSocket(content + wrote, work.size() - wrote);
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
                *outstream << result << std::endl;
                garbage(this);
            });
    std::thread(func, work, socket, outstream, garbage);
}

mis::MISClient::Worker::Worker(TCPSocket *socket, const function<void(mis::MISClient::Worker *)> &callback) : socket(
        socket), garbage(callback) {}

