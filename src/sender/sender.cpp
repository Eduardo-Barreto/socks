#include "sender/sender.hpp"
#include <iostream>

namespace raw_socket {

Sender::Sender(const std::string& interface, uint16_t port) : interface_(interface), port_(port) {
    socket_.bind(interface, port);
    // Set the destination to the same interface and port
    socket_.setDestination(interface, port);
}

void Sender::sendFile(const std::string& filename) {
    try {
        auto fileData = FileUtils::readFile(filename);
        std::cout << "Sending file: " << filename << " (" << fileData.size() << " bytes)" << std::endl;

        // Send the file size first
        uint64_t fileSize = fileData.size();
        socket_.send(&fileSize, sizeof(fileSize));

        // Send the actual file data
        socket_.send(fileData.data(), fileData.size());

        std::cout << "File sent successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error sending file: " << e.what() << std::endl;
        throw;
    }
}

}  // namespace raw_socket