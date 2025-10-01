#pragma once

#include <string>
#include <unordered_map>
#include <optional>

class Database
{
public:
    Database(const std::string& csvPath);
    std::optional<std::string> getVerdict(const std::string& hash) const;
    // Возвращает вердикт, если хеш найден, иначе — std::nullopt

private:
    std::unordered_map<std::string, std::string> hashToVerdict_;
};