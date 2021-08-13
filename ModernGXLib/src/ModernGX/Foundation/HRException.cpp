#include "ModernGX.h"
#include "HRException.h"

MGX::Foundation::HRException::HRException(const char* file, int line, const char* what, HRESULT result) {
    // Get languare
    LCID languageId;
    GetLocaleInfoEx(L"en-US", LOCALE_RETURN_NUMBER | LOCALE_ILANGUAGE, (LPWSTR)&languageId, sizeof(LCID));
    HRESULT hr = GetLastError();

    // Prepare buffer
    char buffer[4096];
    *buffer = 0x00;

    // Exception text from windows (english)
    DWORD res = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
        NULL, result, languageId,
        buffer, 4096, NULL
    );

    // Fallback load system default
    if (!res) {
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, result, NULL,
            buffer, 4096, NULL
        );
    }

    // Clear '\r' from buffer
    char* rChar = strchr(buffer, '\r');
    if (rChar) {
        *rChar = 0x00;
    }

    // Construct the exception
    std::stringstream ss;
    ss << "HRESULT Failed with 0x" << std::hex << result << std::dec << std::endl;
    ss << buffer << std::endl;
    ss << "Action: " << what << std::endl;
    ss << "File: " << file << std::endl;
    ss << "Line: " << line << std::endl;

    // Output to object
    m_text = ss.str();
}

const char* MGX::Foundation::HRException::what() const noexcept {
    return m_text.c_str();
}

void MGX::Foundation::HRException::show() const noexcept {
    MessageBoxA(NULL, m_text.c_str(), "HRESULT Failed", MB_OK | MB_ICONEXCLAMATION);
}
