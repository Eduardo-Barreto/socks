#pragma once

#include <string>
#include "common/socket_utils.hpp"
#include "common/file_utils.hpp"

namespace raw_socket {

class Receiver {
public:
    Receiver(const std::string& interface, uint16_t port);
    void receiveFile(const std::string& outputFilename);

private:
    Socket      socket_;
    std::string interface_;
    uint16_t    port_;
};

}  // namespace raw_socket