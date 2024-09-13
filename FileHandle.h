#pragma once
#define byte win_byte_override
#include <windows.h>
#undef byte
#include <string>
#include <cstdint>
#include <stdexcept>
#ifdef _WIN32
#endif

class FileHandle {
    HANDLE handle;
public:
    FileHandle(const std::wstring& filePath,
               DWORD access = GENERIC_READ,
               DWORD shareMode = FILE_SHARE_READ,
               DWORD creationDisposition = OPEN_EXISTING)
        : handle(CreateFileW(filePath.c_str(),
                             access,
                             shareMode,
                             nullptr,
                             creationDisposition,
                             FILE_ATTRIBUTE_NORMAL,
                             nullptr)) {
        if (handle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to open file: " + std::to_string(GetLastError()));
        }
    }

    ~FileHandle() {
        if (handle != INVALID_HANDLE_VALUE) {
            CloseHandle(handle);
        }
    }

    HANDLE get() const {
        return handle;
    }
};
