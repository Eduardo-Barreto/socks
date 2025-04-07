#pragma once

#include <string>
#include <vector>
#include <system_error>
#include <cstdint>

namespace raw_socket {

class FileError : public std::system_error {
public:
    explicit FileError(const std::string& message) : std::system_error(errno, std::system_category(), message) { }
};

class FileUtils {
public:
    static std::vector<uint8_t> readFile(const std::string& filename);
    static void                 writeFile(const std::string& filename, const std::vector<uint8_t>& data);
};

}  // namespace raw_socket