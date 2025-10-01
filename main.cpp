#include <iostream>
#include <string>
#include <chrono>
#include <locale>
#ifdef _WIN32   
    #include <windows.h>
#endif
#include "core/Scanner.h"


// Парсер аргументов командной строки
bool parse_args(int argc, char* argv[],
    std::string& rootPath,
    std::string& dataPath,
    std::string& logPath) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--path" && i + 1 < argc) {
            rootPath = argv[++i];
        }
        else if (arg == "--base" && i + 1 < argc) {
            dataPath = argv[++i];
        }
        else if (arg == "--log" && i + 1 < argc) {
            logPath = argv[++i];
        }
        else {
            std::cerr << "Неизвестный аргумент: " << arg << "\n";
            return false;
        }
    }

    if (rootPath.empty() || dataPath.empty() || logPath.empty()) {
        std::cerr << "Ошибка: все параметры (--path, --base, --log) обязательны.\n";
        std::cerr << "Пример: scanner.exe --base base.csv --log report.log --path C:\\folder\n";
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::string rootPath, dataPath, logPath;

    if (!parse_args(argc, argv, rootPath, dataPath, logPath)) {
        return EXIT_FAILURE;
    }

    std::cout << "Запуск сканирования...\n";
    std::cout << "Путь: " << rootPath << "\n";
    std::cout << "База хэшей: " << dataPath << "\n";
    std::cout << "Лог: " << logPath << "\n\n";

    Scanner scanner(rootPath, dataPath, logPath);

    auto start = std::chrono::high_resolution_clock::now();
    scanner.start();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = end - start;

    std::cout << "=== Отчёт о сканировании ===\n";
    std::cout << "Обработано файлов: " << scanner.getTotalFiles() << "\n";
    std::cout << "Обнаружено вредоносных: " << scanner.getMaliciousFiles() << "\n";
    std::cout << "Ошибок анализа: " << scanner.getErrors() << "\n";
    std::cout << "Время выполнения: " << std::chrono::duration<double, std::milli>(duration).count() << " мс\n";

    return EXIT_SUCCESS;
}
