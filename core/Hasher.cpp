#include "Hasher.h"
#include <openssl/md5.h>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <vector>

std::string Hasher::computeMD5(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for hashing: " + filePath.u8string());
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);

    // Буфер для чтения (64 КБ — хороший компромисс между скоростью и памятью)
    constexpr size_t bufferSize = 64 * 1024;
    std::vector<unsigned char> buffer(bufferSize);

    while (file) {
        file.read(reinterpret_cast<char*>(buffer.data()), bufferSize);
        std::streamsize bytesRead = file.gcount();
        if (bytesRead > 0) {
            MD5_Update(&ctx, buffer.data(), static_cast<size_t>(bytesRead));
        }
    }

    if (file.bad()) {
        throw std::runtime_error("Error reading file: " + filePath.u8string());
    }

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);

    // Преобразуем байты в hex-строку
    std::ostringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        hexStream << std::setw(2) << static_cast<int>(digest[i]);
    }

    return hexStream.str();
}