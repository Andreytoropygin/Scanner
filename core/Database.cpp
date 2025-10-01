#include "Database.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

Database::Database(const std::string& csvPath)
{
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open database file: " + csvPath);
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t sepPos = line.find(';');
        if (sepPos == std::string::npos) {
            continue;
        }

        std::string hash = line.substr(0, sepPos);
        std::string verdict = line.substr(sepPos + 1);

        hashToVerdict_[hash] = verdict;
    }
}

std::optional<std::string> Database::getVerdict(const std::string& hash) const
{
    auto it = hashToVerdict_.find(hash);
    if (it != hashToVerdict_.end()) {
        return it->second;
    }
    return std::nullopt;
}
