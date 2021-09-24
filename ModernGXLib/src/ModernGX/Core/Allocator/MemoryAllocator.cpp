#include "ModernGX.h"
#include "MemoryAllocator.h"

MGX::Core::Allocator::MemoryAllocator::MemoryAllocator(UINT32 slotCount, UINT slotSize, IAllocator* ptrBaseAllocator) :
    m_ptrBaseAllocator(ptrBaseAllocator),
    m_ac(slotCount, ptrBaseAllocator),
    m_slotSize(slotSize)
{
    // Allocate memory
    m_ptrMemory = m_ptrBaseAllocator->Allocate((UINT64)slotSize * slotCount);
    if (m_ptrMemory)
    {
        m_slotCount = slotCount;
    }
}

MGX::Core::Allocator::MemoryAllocator::~MemoryAllocator()
{
    // Free memory
    if (m_ptrMemory)
    {
        m_ptrBaseAllocator->Free(m_ptrMemory);
    }
}

MGX::Core::Allocator::MemoryAllocator& MGX::Core::Allocator::MemoryAllocator::operator=(MemoryAllocator&& other) noexcept
{
    // Destruct this
    this->~MemoryAllocator();

    // Move
    m_ac = std::move(other.m_ac);

    // Copy
    m_ptrBaseAllocator = other.m_ptrBaseAllocator;
    m_ptrMemory = other.m_ptrMemory;
    m_slotSize = other.m_slotSize;
    m_slotCount = other.m_slotCount;
    m_allocatorUsage = other.m_allocatorUsage;

    // Clear other
    other.m_ptrMemory = nullptr;
    other.m_slotCount = 0;
    other.m_allocatorUsage = 0;

    return *this;
}

void* MGX::Core::Allocator::MemoryAllocator::Allocate(size_t size) noexcept
{
    void* ptrMem = nullptr;

    // Get slot count
    UINT32 slotCount = (UINT32)((size + m_slotSize - 1) / m_slotSize);

    // Allocate slot
    UINT32 slotOffset = m_ac.SlotAlloc(slotCount);
    if (slotOffset != UINT32_MAX)
    {
        // Set pointer
        ptrMem = (void*)( ((UINT64)m_ptrMemory) + m_slotSize * slotOffset );

        // Debug memset
        #ifdef _DEBUG
        memset(ptrMem, 0xEA, (UINT64)m_slotSize * slotCount);
        #endif

        // Increment usage
        m_allocatorUsage += (UINT64)m_slotSize * slotCount;
    }

    return ptrMem;
}

void MGX::Core::Allocator::MemoryAllocator::Free(void* memory) noexcept
{
    // Find slot index
    UINT32 slotIdx = (UINT32)( ( ((UINT64)memory) - ((UINT64)m_ptrMemory) ) / m_slotSize );

    // Free slot (update usage)
    m_allocatorUsage -= m_ac.SlotFree(slotIdx) * m_slotSize;
}

UINT64 MGX::Core::Allocator::MemoryAllocator::GetByteSize() const noexcept
{
    return (UINT64)m_slotSize * m_slotCount;
}

UINT64 MGX::Core::Allocator::MemoryAllocator::GetByteUsage() const noexcept
{
    return m_allocatorUsage;
}
