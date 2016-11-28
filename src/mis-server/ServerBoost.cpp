//
// Created by CIJhn on 11/27/2016.
//

#include "mis-instructions/default_instructions.h"
#include "mis-server/MISServer.h"

mis::MISServer *createDefaultServer() {
    TCPServerSocket *socket = new TCPServerSocket("127.0.0.1", 450, 16);
    mis::Parser::Builder builder;
    mis::register_default(builder);
    return new mis::MISServer(socket, new mis::VirtualMachine(), builder.build());
}

int main(int argc, char *argv[]) {
    mis::MISServer *server = createDefaultServer();
    server->start();
    std::string input;
    while (!(std::cin >> input).eof()) {
        if (input == "dump") {
            auto history = server->getHistory();
            std::cout << "totally " + history.size() << " records." << std::endl;
            for (auto record: history)
                std::cout << "[" + record->getStartTime() + "] " +
                             record->getIp() + " spent " + record->getDuration() + "milliseconds." << std::endl;
        } else {
            std::cout << "unknown command! view history: dump, exit: ctrl+d" << std::endl;
        }
    }
    std::cout << "Terminating the server..." << std::endl;
    server->terminated();
    std::cout << "Server terminated." << std::endl;
}