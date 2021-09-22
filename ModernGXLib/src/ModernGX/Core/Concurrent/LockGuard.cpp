#include "ModernGX.h"
#include "LockGuard.h"

MGX::Core::Concurrent::LockGuard::LockGuard(ILock* ptrLock) noexcept :
    m_ptrLock(ptrLock)
{
    if (m_ptrLock)
    {
        m_ptrLock->Aquire();
    }
}

MGX::Core::Concurrent::LockGuard::LockGuard(ILock& refLock) noexcept :
    m_ptrLock(&refLock)
{
    refLock.Aquire();
}

MGX::Core::Concurrent::LockGuard::~LockGuard()
{
    if (m_ptrLock)
    {
        m_ptrLock->Release();
    }
}

bool MGX::Core::Concurrent::LockGuard::IsValid() const noexcept
{
    return m_ptrLock != nullptr;
}

void MGX::Core::Concurrent::LockGuard::Close() noexcept
{
    if (m_ptrLock)
    {
        m_ptrLock->Release();
        m_ptrLock = nullptr;
    }
}

MGX::Core::Concurrent::LockGuard::operator bool()
{
    return m_ptrLock != nullptr;
}
