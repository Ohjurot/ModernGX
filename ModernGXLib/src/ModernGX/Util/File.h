#pragma once

#include <ModernGX.h>
#include <initializer_list>
#include <type_traits>

namespace MGX::Util
{
    // File helper class
    class File
    {
        // Static helpers
        public:
            // Checks if a file exists
            static bool Exists(const WCHAR* fileName) noexcept;

            // Find one file (helper for many)
            static File Open(const WCHAR* fileName, const WCHAR* directory)
            {
                // Build file buffer
                WCHAR buffer[2048];
                wcscpy_s<2048>(buffer, directory);
                wcscat_s<2048>(buffer, fileName);

                // File from path
                return File(buffer);
            }

            // Find one of many files
            template<typename... T>
            static File Open(const WCHAR* fileName, T... basePaths) noexcept
            {
                // Find first match
                for (auto basePath : std::initializer_list<const WCHAR*>{ basePaths... })
                {
                    // Open file and check
                    File f = Open(fileName, basePath);
                    if (f)
                    {
                        return f;
                    }
                }
                    
                // Empty file
                return File();
            }

        // Dynamic class
        public:
            // Constructors
            File() = default;
            File(const File& other);
            File(const WCHAR* fileName);

            // Destruct
            ~File();

            // Open / Closing
            bool Open(bool write = false, bool shared = true) noexcept;
            void Close() noexcept;

            // Get file size
            UINT64 Size() const noexcept;

            // File seeking
            inline UINT64 GetFilePointerPos() const noexcept
            {
                return m_fileCursor;
            }
            // Set file pointer pos
            void SetFilePointerPos(UINT64 pos) noexcept;

            // Read file functions
            bool Read(void* buffer, UINT32 numBytes) noexcept;
            bool Read(void* buffer, UINT64 numBytes) noexcept;

            // Easy read
            template<typename T>
            bool ReadT(T* buffer, UINT64 count = 1) noexcept
            {
                return Read(buffer, sizeof(T) * count);
            }

            // Check if file exists
            inline bool Exists() const noexcept
            {
                return File::Exists(m_namBuffer);
            }

            // Check if file is open
            inline bool IsOpen() const noexcept
            {
                return m_hFile != INVALID_HANDLE_VALUE;
            }

            // Boolean conversion
            inline operator bool() const noexcept
            {
                return Exists();
            }

        private:
            // Name / Path buffer
            WCHAR m_namBuffer[2048] = {};

            // File handle
            HANDLE m_hFile = INVALID_HANDLE_VALUE;

            // State helpers
            UINT64 m_fileCursor = 0;
            bool m_readOnly = TRUE;
            bool m_shared = TRUE;
    };
}
