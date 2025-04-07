#include "common/file_utils.hpp"
#include <fstream>
#include <stdexcept>
#include <cstdint>

namespace raw_socket {

std::vector<uint8_t> FileUtils::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw FileError("Failed to open file for reading: " + filename);
    }

    const auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw FileError("Failed to read file: " + filename);
    }

    return buffer;
}

void FileUtils::writeFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw FileError("Failed to open file for writing: " + filename);
    }

    if (!file.write(reinterpret_cast<const char*>(data.data()), data.size())) {
        throw FileError("Failed to write file: " + filename);
    }
}

}  // namespace raw_socket