#include "ModernGX.h"
#include "SpinLock.h"

bool MGX::Core::Concurrent::SpinLock::TryAquire() noexcept
{
    return !m_lockFlag.test_and_set(std::memory_order_acquire);
}

void MGX::Core::Concurrent::SpinLock::Release() noexcept
{
    m_lockFlag.clear(std::memory_order_release);
}
