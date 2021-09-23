#include "ModernGX.h"
#include "StackMemoryAccelerator.h"

MGX::Core::Allocator::StackMemoryAccelerator::StackMemoryAccelerator(UINT32 size) :
    m_count(size)
{

}

MGX::Core::Allocator::StackMemoryAccelerator& MGX::Core::Allocator::StackMemoryAccelerator::operator=(StackMemoryAccelerator&& other) noexcept
{
    // Only if not valid
    if (!m_count)
    {
        // Copy
        m_count = other.m_count;
        m_head = other.m_head;

        // Invalidate
        other.m_count = 0;
        other.m_head = 0;
    }

    return *this;
}

MGX::Core::Allocator::StackMemoryAccelerator::operator bool() const noexcept
{
    return m_count > 0;
}

UINT32 MGX::Core::Allocator::StackMemoryAccelerator::SlotAlloc(UINT32 size) noexcept
{
    UINT32 slot = UINT32_MAX;

    // Check if range matches
    if (m_head + size <= m_count)
    {
        slot = m_head;
        m_head += size;
    }

    return slot;
}

void MGX::Core::Allocator::StackMemoryAccelerator::Back(UINT32 idx) noexcept
{
    if (idx <= m_head)
    {
        m_head = idx;
    }
}

void MGX::Core::Allocator::StackMemoryAccelerator::Reset() noexcept
{
    m_head = 0;
}
