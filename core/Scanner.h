#pragma once

#ifdef _WIN32
    #ifdef CORE_EXPORTS
        #define CORE_API __declspec(dllexport)
    #else
        #define CORE_API __declspec(dllimport)
    #endif
#else
    #define CORE_API 
#endif   


#include "Hasher.h"
#include "Database.h"
#include "Logger.h"
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <filesystem>

class CORE_API Scanner
{
public:
    Scanner(const std::string& rootPath, const std::string& dataPath, const std::string& logPath);
    ~Scanner();

    // ������ ������������
    void start();

    // ������� ��� ����������
    size_t getTotalFiles() const { return total_files_.load(); }
    size_t getMaliciousFiles() const { return malicious_files_.load(); }
    size_t getErrors() const { return errors_.load(); }

private:
    void iterateDirectory(const std::filesystem::path& rootPath);   // ����� ���������� � ���������� ������� ������ �� ���������
    void workerThread();                                            // ������ ������
    void processFile(const std::filesystem::path& filePath);        // ��������� ����� � ������

    // ������� ������
    std::queue<std::filesystem::path> fileQueue_;
    mutable std::mutex queueMutex_;
    mutable std::mutex coutMutex_;

    // ��� �������
    std::vector<std::thread> workers_;

    std::string rootPath_;      // ���� � ����������

    Database database_;         // ���� �����
    Logger logger_;             // ������

    // ����������
    std::atomic<size_t> total_files_{ 0 };      // ������� ������������ ������
    std::atomic<size_t> malicious_files_{ 0 };  // ������� ����������� ������
    std::atomic<size_t> errors_{ 0 };           // ������� ������
};