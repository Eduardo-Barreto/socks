#include "receiver/receiver.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <interface> <port> <output_filename>" << std::endl;
        return 1;
    }

    try {
        const std::string interface = argv[1];
        const uint16_t    port = static_cast<uint16_t>(std::stoi(argv[2]));
        const std::string outputFilename = argv[3];

        raw_socket::Receiver receiver(interface, port);
        receiver.receiveFile(outputFilename);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}