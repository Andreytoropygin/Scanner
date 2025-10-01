cmake_minimum_required(VERSION 3.14)
project(Scanner)

# Требуем C++17 (для std::filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем экспорт всех символов на Windows (для DLL)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_executable(Scanner main.cpp)

# Подключаем субдиректории
add_subdirectory(core)
#add_subdirectory(tests)

target_link_libraries(Scanner PRIVATE core)
