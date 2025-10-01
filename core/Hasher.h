#pragma once

#include <string>
#include <filesystem>

class Hasher
{
public:
    static std::string computeMD5(const std::filesystem::path& filePath);   
    // Возвращает MD5 в виде строки hex. При ошибке чтения — выбрасывает std::runtime_error или возвращает пустую строку
};