#include "Logger.h"
#include <fstream>

Logger::Logger(const std::string& logPath) : logPath_(logPath) {};

void Logger::logMalicious(const std::filesystem::path& filePath, const std::string& hash, const std::string& verdict)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::ofstream logFile(logPath_, std::ios::app);
    if (logFile.is_open()) {
        logFile << "[PATH] " << filePath.u8string() << "\n";
        logFile << "[HASH] " << hash << "\n";
        logFile << "[VERDICT] " << verdict << "\n\n";
    }
}