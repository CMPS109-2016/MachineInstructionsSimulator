
#include "mis-core/strutil.h"
#include "mis-client/MISClient.h"
#include <sys/stat.h>

inline bool exist(std::string &filename) {
    struct stat status;
    return stat(filename.c_str(), &status) == 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "usage: mis <ip> [-out=outFileName] <filename>" << std::endl;
        return 0;
    }

    std::string ip(argv[1]);
    if (ip == "Localhost" || ip == "localhost")
        ip = "127.0.0.1";
    std::cout << "Starting MIS client..." << std::endl;
    mis::MISClient *client = new mis::MISClient(ip);
    std::string filename(argv[2]);
    basic_ostream<char> *stream = &std::cout;
    auto pos = filename.find("-out=");
    if (pos != filename.npos) {
        filename = filename.substr(pos);
        if (exist(filename)) {
            stream = new std::ofstream(filename);
            if (argc == 3)
                filename = argv[3];
            else {
                std::cout << "Require third argument as the file containing the code!" << std::endl;
                return 0;
            }
        } else {
            std::cout << "Cannot find file [" + filename + "]!" << std::endl;
            return 0;
        }
    }

    std::stringstream bufferStream;
    if (exist(filename)) {
        std::cout << "Reading the file [" + filename + "]..." << std::endl;
        std::string content(mis::readFileToString(filename));
        std::cout << "Query compilation to the file [" + filename + "]..." << std::endl;
        bool suc = client->queryCompile(content, &bufferStream, true);
        if (suc) {
            std::string result;
            bufferStream >> result;
            (*stream) << result;
        }
    } else {
        std::cout << "Cannot find file [" + filename + "]!" << std::endl;
        return 0;
    }


    if (stream != &std::cout)
        delete (stream);
    return 0;
}






