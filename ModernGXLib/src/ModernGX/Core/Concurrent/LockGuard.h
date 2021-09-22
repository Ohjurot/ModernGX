#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Concurrent/ILock.h>

namespace MGX::Core::Concurrent
{
    // Guard for locks
    class LockGuard
    {
        public:
            // Construct
            LockGuard() = default;
            LockGuard(const LockGuard&) = delete;
            LockGuard(ILock* ptrLock) noexcept;
            LockGuard(ILock& refLock) noexcept;

            // Destruct
            ~LockGuard();

            // Checks if lock is valid (aquired)
            bool IsValid() const noexcept;

            // Manual close function
            void Close() noexcept;

            // Delete assign
            LockGuard& operator=(const LockGuard&) = delete;

            // Check if guard is valid
            operator bool();

        private:
            // Pointer to manged lock
            ILock* m_ptrLock = nullptr;
    };
}
