#include "ModernGX.h"
#include "Memory.h"

char* MGX::StrAllocateCopyA(Core::IAllocator* ptrAllocator, const char* ptrSource) noexcept
{
    // Get string length
    size_t strLen = strlen(ptrSource);

    // Allocate memory for string
    char* newStr = ptrAllocator->TAllocate<char>(strLen + 1);
    
    // Copy
    if (newStr)
    {
        // Copy string
        strcpy_s(newStr, strLen + 1, ptrSource);
    }

    // Return string pointer
    return newStr;
}

wchar_t* MGX::StrAllocateCopyW(Core::IAllocator* ptrAllocator, const wchar_t* ptrSource) noexcept
{
    // Get string length
    size_t strLen = wcslen(ptrSource);

    // Allocate memory for string
    wchar_t* newStr = ptrAllocator->TAllocate<wchar_t>(strLen + 1);

    // Copy
    if (newStr)
    {
        // Copy string
        wcscpy_s(newStr, strLen + 1, ptrSource);
    }

    // Return string pointer
    return newStr;
}
