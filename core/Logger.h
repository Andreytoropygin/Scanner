#pragma once

#include <string>
#include <filesystem>
#include <mutex>

class Logger
{
public:
    Logger(const std::string& logPath);
    void logMalicious(const std::filesystem::path& filePath, const std::string& hash, const std::string& verdict);

private:
    std::string logPath_;
    mutable std::mutex mutex_;
};