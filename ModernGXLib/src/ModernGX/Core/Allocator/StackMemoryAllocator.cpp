#include "ModernGX.h"
#include "StackMemoryAllocator.h"

MGX::Core::Allocator::StackMemoryAllocator::StackMemoryAllocator(UINT32 slotCount, IAllocator* ptrBaseAllocator) :
    m_ptrBaseAllocator(ptrBaseAllocator),
    m_slotSize(1),
    m_slotCount(slotCount),
    m_ac(slotCount)
{
    // Allocate memory
    m_ptrMemory = m_ptrBaseAllocator->Allocate(slotCount);
}

MGX::Core::Allocator::StackMemoryAllocator::StackMemoryAllocator(UINT32 slotCount, UINT32 slotSize, IAllocator* ptrBaseAllocator) :
    m_ptrBaseAllocator(ptrBaseAllocator),
    m_slotSize(slotSize),
    m_slotCount(slotCount),
    m_ac(slotCount)
{
    // Allocate memory
    m_ptrMemory = m_ptrBaseAllocator->Allocate(slotSize * slotCount);
}

MGX::Core::Allocator::StackMemoryAllocator::~StackMemoryAllocator()
{
    if (m_ptrMemory)
    {
        m_ptrBaseAllocator->Free(m_ptrMemory);
    }
}

MGX::Core::Allocator::StackMemoryAllocator& MGX::Core::Allocator::StackMemoryAllocator::operator=(StackMemoryAllocator&& other) noexcept
{
    if (!m_ptrMemory)
    {
        // Move other
        m_ac = std::move(other.m_ac);

        // Copy other
        m_ptrBaseAllocator = other.m_ptrBaseAllocator;
        m_ptrMemory = other.m_ptrMemory;
        m_slotSize = other.m_slotSize;
        m_slotCount = other.m_slotCount;
        m_usage = other.m_usage;

        // Invalidate other
        other.m_ptrMemory = nullptr;
        other.m_slotCount = 0;
        other.m_usage = 0;
    }

    return *this;
}

void MGX::Core::Allocator::StackMemoryAllocator::Reset() noexcept
{
    // Reset
    m_ac.Reset();
    m_usage = 0;
}

void* MGX::Core::Allocator::StackMemoryAllocator::Allocate(size_t size) noexcept
{
    void* ptrMem = nullptr;

    // Compute number of slots required
    UINT32 slotCount = (UINT32)((size + m_slotSize - 1) / m_slotSize);

    // Allocate slots
    UINT32 slotIdx = m_ac.SlotAlloc(slotCount);
    if (slotIdx != UINT32_MAX)
    {
        ptrMem = (void*)(((UINT64)m_ptrMemory) + slotIdx * m_slotSize);

        // Debug memset
        #ifdef _DEBUG
        memset(ptrMem, 0xEA, size);
        #endif
    }

    return ptrMem;
}

void MGX::Core::Allocator::StackMemoryAllocator::Free(void* memory) noexcept
{

}

UINT64 MGX::Core::Allocator::StackMemoryAllocator::GetByteSize() const noexcept
{
    return (UINT64)m_slotCount * m_slotSize;
}

UINT64 MGX::Core::Allocator::StackMemoryAllocator::GetByteUsage() const noexcept
{
    return m_usage;
}
