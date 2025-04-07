#include "sender/sender.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <interface> <port> <filename>" << std::endl;
        return 1;
    }

    try {
        const std::string interface = argv[1];
        const uint16_t    port = static_cast<uint16_t>(std::stoi(argv[2]));
        const std::string filename = argv[3];

        raw_socket::Sender sender(interface, port);
        sender.sendFile(filename);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}