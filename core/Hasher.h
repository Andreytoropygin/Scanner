#pragma once

#include <string>
#include <filesystem>

class Hasher
{
public:
    static std::string computeMD5(const std::filesystem::path& filePath);   
    // ���������� MD5 � ���� ������ hex. ��� ������ ������ � ����������� std::runtime_error ��� ���������� ������ ������
};