#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

// returns pointer to memory that needs to be freed with `LocalFree()`
LPCTSTR format_message_for_error_code(DWORD error){
    LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        ((LPTSTR)&lpMsgBuf),
        0,
        NULL );
    return (LPCTSTR)lpMsgBuf;
}


// Helper macro to avoid freeing memory manually (or using unique_ptr)
// Should work in C and C++ alike
#define with_error_message(error, msg, code) do {     LPTSTR msg; \
    FormatMessage( \
        FORMAT_MESSAGE_ALLOCATE_BUFFER | \
        FORMAT_MESSAGE_FROM_SYSTEM | \
        FORMAT_MESSAGE_IGNORE_INSERTS, \
        NULL, \
        (DWORD)(error), \
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
        (LPTSTR) &msg, \
        0, \
        NULL ); \
        do { code ; } while(0); \
    LocalFree((msg)); \
    } while (0);

// use 
std::wstring LocaleEx::GetString(LCTYPE type)
{
    auto len = GetLocaleInfoEx(localeName.c_str(), type, nullptr, 0);
    if (!len)
    {
        with_error_message(GetLastError(), msg, {
            throw std::logic_error(msg);
        });
    }
    std::wstring buffer(len + 1, L'\0');
    GetLocaleInfoEx(localeName.c_str(), type, (LPWSTR)buffer.data(), len);
    return buffer;
}

#include <memory>

// With C++ one can also do
auto format_message_for_error_code_smart(DWORD error) {
    return std::unique_ptr{format_message_for_error_code(error), LocalFree};
}

// or, with COM
std::wstring _(){
    _com_error e(HRESULT_FROM_WIN32(GetLastError()));
    return std::wstring(e.ErrorMessage());
}

// BUT it's all not needed anymore because now it's part of stdlib!
#include <system_error>

std::string message = std::system_category().message(err);