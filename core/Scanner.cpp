#include "Scanner.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

Scanner::Scanner(const std::string& rootPath, const std::string& dataPath, const std::string& logPath)
    : rootPath_(rootPath)
    , database_(dataPath)
    , logger_(logPath) {}

Scanner::~Scanner()
{
    for (auto& t : workers_) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void Scanner::start()
{
    // ����� ���������� � ���������� �������
    try {
        iterateDirectory(rootPath_);
    }
    catch (const std::filesystem::filesystem_error& e) {
        ++errors_;
        std::cerr << "Iteration error: " << e.what() << std::endl;
    }

    // ���� ������� �����, �� ��������� ������
    if (fileQueue_.empty()) {
        return;
    }

    // ������ ���� �������
    size_t threadCount_ = std::thread::hardware_concurrency() - 1;
    if (threadCount_ > 1) {
        workers_.reserve(threadCount_);
        for (size_t i = 0; i < threadCount_; ++i) {
            workers_.emplace_back(&Scanner::workerThread, this);
        }

        // �������� ���������� ���� �������
        for (auto& t : workers_) {
            if (t.joinable()) {
                // {
                //     std::unique_lock<std::mutex> lock(coutMutex_);
                //     std::cout << t.get_id() << " joined\n";
                // }
                t.join();
            }
        }
    }
    else {
        Scanner::workerThread();
    }
}

void Scanner::iterateDirectory(const std::filesystem::path& rootPath)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath)) {
        if (entry.is_regular_file()) {
            fileQueue_.push(entry.path());
        }
    }
    
}

void Scanner::workerThread()
{
    while (true) {
        std::filesystem::path fileToProcess;

        {
            std::unique_lock<std::mutex> lock(queueMutex_);

            if (fileQueue_.empty()) {
                break;
            }

            fileToProcess = std::move(fileQueue_.front());
            fileQueue_.pop();
        }
        
        processFile(fileToProcess);
    }
}

// ��������� �����
void Scanner::processFile(const std::filesystem::path& filePath)
{
    ++total_files_;

    std::string hash;
    try
    {
        // ����������� ����� 
        hash = Hasher::computeMD5(filePath);
        // {
        //     std::unique_lock<std::mutex> lock(coutMutex_);
        //     std::cout << hash << " by " << std::this_thread::get_id() << "\n";
        // }
        if (hash.empty()) {
            throw std::runtime_error("Empty hash returned");
        }
    }
    catch (const std::exception&)
    {
        ++errors_;
        return;
    }

    // �������� ���� � ����
    auto verdict = database_.getVerdict(hash);
    if (verdict.has_value())
    {
        // ������������ ������������ �����
        logger_.logMalicious(filePath, hash, verdict.value());
        ++malicious_files_;
    }
}
