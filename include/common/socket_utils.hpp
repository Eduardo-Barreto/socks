#pragma once

#include <string>
#include <system_error>
#include <cstdint>
#include <netinet/ip.h>
#include <netinet/in.h>

namespace raw_socket {

class SocketError : public std::system_error {
public:
    explicit SocketError(const std::string& message) : std::system_error(errno, std::system_category(), message) { }
};

// Custom protocol number (must be > 0 and not used by other protocols)
constexpr int CUSTOM_PROTOCOL = 253;

// IP header structure
struct IPHeader {
    uint8_t  version_ihl;   // Version (4 bits) + Internet header length (4 bits)
    uint8_t  tos;           // Type of service
    uint16_t total_length;  // Total length
    uint16_t id;            // Identification
    uint16_t frag_off;      // Fragment offset
    uint8_t  ttl;           // Time to live
    uint8_t  protocol;      // Protocol
    uint16_t check;         // Checksum
    uint32_t saddr;         // Source address
    uint32_t daddr;         // Destination address
};

class Socket {
public:
    Socket();
    ~Socket();

    // Delete copy constructor and assignment operator
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // Move constructor and assignment operator
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    void bind(const std::string& interface, uint16_t port);
    void setDestination(const std::string& dest_ip, uint16_t dest_port);
    void send(const void* data, size_t length);
    void receive(void* buffer, size_t length);
    void close();

private:
    int socket_fd_{-1};

    struct sockaddr_in dest_addr_ { };

    uint16_t calculateChecksum(const void* data, size_t length);
};

}  // namespace raw_socket