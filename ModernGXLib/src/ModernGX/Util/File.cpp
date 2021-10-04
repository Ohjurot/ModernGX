#include "ModernGX.h"
#include "File.h"

bool MGX::Util::File::Exists(const WCHAR* fileName) noexcept
{
    bool exists = false;

    // Open file with all share (do not block file)
    HANDLE hFile = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        // Set helper and close file
        exists = true;
        CloseHandle(hFile);
    }

    return exists;
}

MGX::Util::File::File(const File& other)
{
    // Destruct this file
    this->~File();

    // Copy file name
    wcscpy_s<2048>(m_namBuffer, other.m_namBuffer);
}

MGX::Util::File::File(const WCHAR* fileName)
{
    // Copy file name
    wcscpy_s<2048>(m_namBuffer, fileName);
}

MGX::Util::File::~File()
{
    Close();
}

bool MGX::Util::File::Open(bool write, bool shared) noexcept
{
    // Open file
    m_hFile = CreateFile(m_namBuffer, write ? GENERIC_ALL : GENERIC_READ, write ? NULL : FILE_SHARE_READ, NULL, write ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (IsOpen())
    {
        // Get current cursor positon
        LARGE_INTEGER filePos;
        filePos.QuadPart = 0;
        filePos.LowPart = SetFilePointer(m_hFile, filePos.LowPart, &filePos.HighPart, FILE_CURRENT);

        // Store
        m_fileCursor = filePos.QuadPart;
        m_shared = shared;
        m_readOnly = !write;

        // Open 
        return true;
    }

    // Not open
    return false;
}

void MGX::Util::File::Close() noexcept
{
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
        m_fileCursor = 0;
    }
}

UINT64 MGX::Util::File::Size() const noexcept
{
    if (IsOpen())
    {
        // Get size
        LARGE_INTEGER fileSize;
        fileSize.LowPart = GetFileSize(m_hFile, (DWORD*)&fileSize.HighPart);

        return fileSize.QuadPart;
    }

    return 0;
}

void MGX::Util::File::SetFilePointerPos(UINT64 pos) noexcept
{
    if (IsOpen())
    {
        // Set position
        LARGE_INTEGER filePos;
        filePos.QuadPart = pos;
        SetFilePointer(m_hFile, filePos.LowPart, &filePos.HighPart, FILE_BEGIN);

        // Update
        m_fileCursor = pos;
    }
}

bool MGX::Util::File::Read(void* buffer, UINT32 numBytes) noexcept
{
    bool result = false;

    // Read file
    if (IsOpen() && ReadFile(m_hFile, buffer, numBytes, NULL, NULL))
    {
        // Incrment file pointer
        m_fileCursor += numBytes;
        result = true;
    }

    return result;
}

bool MGX::Util::File::Read(void* buffer, UINT64 numBytes) noexcept
{
    if (IsOpen())
    {
        // Current offset
        UINT64 offset = 0;
        BYTE* ptrBytes = (BYTE*)buffer;

        // While winapi not fitting
        while (numBytes > UINT32_MAX)
        {
            // Read bytes
            if (!Read(&ptrBytes[offset], (UINT32)UINT32_MAX))
            {
                // Failed
                return false;
            }

            // Increment offset
            offset += UINT32_MAX;
            numBytes -= UINT32_MAX;
        }

        // last read
        if (numBytes)
        {
            return Read(&ptrBytes[offset], (UINT32)numBytes);
        }

        //  Ok
        return true;
    }

    // Failed
    return false;
}
