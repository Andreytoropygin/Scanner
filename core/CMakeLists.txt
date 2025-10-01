# ������� shared ���������� (DLL)
add_library(core SHARED
    Scanner.cpp
    Scanner.h
    Hasher.cpp
    Hasher.h
    Logger.cpp
    Logger.h
    Database.cpp
    Database.h
)

if(WIN32)
    set_target_properties(core PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenSSL REQUIRED)

target_link_libraries(core PRIVATE OpenSSL::SSL OpenSSL::Crypto)
