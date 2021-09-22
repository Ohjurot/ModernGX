#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Concurrent/ILock.h>

#include <atomic>

namespace MGX::Core::Concurrent
{
    // Lightweight atomic user mode lock
    class SpinLock : public ILock
    {
        public:
            // Setup default/delete construct
            SpinLock() = default;
            SpinLock(const SpinLock&) = delete;
            
            // Implement interface
            bool TryAquire() noexcept override;
            void Release() noexcept override;

            // Delete assign
            SpinLock& operator=(const SpinLock&) = delete;

        private:
            std::atomic_flag m_lockFlag = ATOMIC_FLAG_INIT;
    };
}
