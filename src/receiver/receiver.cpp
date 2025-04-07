#include "receiver/receiver.hpp"
#include <iostream>
#include <cstring>

namespace raw_socket {

Receiver::Receiver(const std::string& interface, uint16_t port) : interface_(interface), port_(port) {
    socket_.bind(interface, port);
    // Set the destination to the same interface and port
    socket_.setDestination(interface, port);
}

void Receiver::receiveFile(const std::string& outputFilename) {
    try {
        // Buffer to receive the file size (including IP header)
        std::vector<uint8_t> sizeBuffer(sizeof(struct iphdr) + sizeof(uint64_t));

        // Receive the file size
        socket_.receive(sizeBuffer.data(), sizeBuffer.size());

        // Extract the file size from after the IP header
        uint64_t fileSize;
        memcpy(&fileSize, sizeBuffer.data() + sizeof(struct iphdr), sizeof(fileSize));

        std::cout << "Receiving file of size: " << fileSize << " bytes" << std::endl;

        // Create buffer for the file data (including IP header)
        std::vector<uint8_t> buffer(sizeof(struct iphdr) + fileSize);

        // Receive the actual file data
        socket_.receive(buffer.data(), buffer.size());

        // Extract the file data from after the IP header
        std::vector<uint8_t> fileData(fileSize);
        memcpy(fileData.data(), buffer.data() + sizeof(struct iphdr), fileSize);

        // Write the received data to file
        FileUtils::writeFile(outputFilename, fileData);

        std::cout << "File received and saved as: " << outputFilename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error receiving file: " << e.what() << std::endl;
        throw;
    }
}

}  // namespace raw_socket