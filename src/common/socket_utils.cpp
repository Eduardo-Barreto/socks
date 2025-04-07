#include "common/socket_utils.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace raw_socket {

Socket::Socket() {
    // Create raw socket with our custom protocol
    socket_fd_ = socket(AF_INET, SOCK_RAW, CUSTOM_PROTOCOL);
    if (socket_fd_ < 0) {
        throw SocketError("Failed to create raw socket");
    }

    // Enable IP_HDRINCL to allow us to build our own IP header
    int opt = 1;
    if (setsockopt(socket_fd_, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt)) < 0) {
        close();
        throw SocketError("Failed to set IP_HDRINCL");
    }
}

Socket::~Socket() {
    close();
}

Socket::Socket(Socket&& other) noexcept : socket_fd_(other.socket_fd_) {
    other.socket_fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        close();
        socket_fd_ = other.socket_fd_;
        other.socket_fd_ = -1;
    }
    return *this;
}

void Socket::bind(const std::string& interface, uint16_t port) {
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(interface.c_str());
    addr.sin_port = htons(port);

    if (::bind(socket_fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw SocketError("Failed to bind socket");
    }
}

void Socket::setDestination(const std::string& dest_ip, uint16_t dest_port) {
    std::memset(&dest_addr_, 0, sizeof(dest_addr_));
    dest_addr_.sin_family = AF_INET;
    dest_addr_.sin_addr.s_addr = inet_addr(dest_ip.c_str());
    dest_addr_.sin_port = htons(dest_port);
}

void Socket::send(const void* data, size_t length) {
    // Create IP header
    struct iphdr ip_header;
    std::memset(&ip_header, 0, sizeof(ip_header));

    // Set IP header fields
    ip_header.version = 4;
    ip_header.ihl = 5;  // 5 * 32-bit words
    ip_header.tot_len = htons(sizeof(iphdr) + length);
    ip_header.id = htons(54321);
    ip_header.frag_off = 0;
    ip_header.ttl = 64;
    ip_header.protocol = CUSTOM_PROTOCOL;
    ip_header.saddr = inet_addr("127.0.0.1");      // Source IP
    ip_header.daddr = dest_addr_.sin_addr.s_addr;  // Destination IP
    ip_header.check = 0;                           // Checksum will be calculated by the kernel

    // Create packet buffer
    std::vector<uint8_t> packet(sizeof(iphdr) + length);
    std::memcpy(packet.data(), &ip_header, sizeof(iphdr));
    std::memcpy(packet.data() + sizeof(iphdr), data, length);

    // Send the packet
    if (sendto(socket_fd_, packet.data(), packet.size(), 0, (struct sockaddr*)&dest_addr_, sizeof(dest_addr_)) < 0) {
        throw SocketError("Failed to send data");
    }
}

void Socket::receive(void* buffer, size_t length) {
    struct sockaddr_in sender_addr;
    socklen_t          sender_len = sizeof(sender_addr);

    ssize_t received = recvfrom(socket_fd_, buffer, length, 0, (struct sockaddr*)&sender_addr, &sender_len);
    if (received < 0) {
        throw SocketError("Failed to receive data");
    }
}

void Socket::close() {
    if (socket_fd_ != -1) {
        ::close(socket_fd_);
        socket_fd_ = -1;
    }
}

}  // namespace raw_socket
