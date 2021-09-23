#include "ModernGX.h"
#include "FastPoolAllocator.h"

MGX::Core::Allocator::FastPoolAllocator::FastPoolAllocator(UINT32 slotCount, UINT32 slotSize, IAllocator* ptrBaseAllocator) :
    m_ptrBaseAllocator(ptrBaseAllocator),
    m_slotSize(slotSize),
    m_ac(slotCount, ptrBaseAllocator)
{
    // Allocate base memory
    m_baseAddress = m_ptrBaseAllocator->Allocate((size_t)slotSize * slotCount);
}

MGX::Core::Allocator::FastPoolAllocator::~FastPoolAllocator()
{
    // Release memory if present
    if (m_baseAddress)
        m_ptrBaseAllocator->Free(m_baseAddress);
}

MGX::Core::Allocator::FastPoolAllocator& MGX::Core::Allocator::FastPoolAllocator::operator=(FastPoolAllocator&& other) noexcept
{
    // Copy members
    m_ac = std::move(other.m_ac);
    m_baseAddress = other.m_baseAddress;
    m_slotSize = other.m_slotSize;
    m_ptrBaseAllocator = other.m_ptrBaseAllocator;

    // Clear others pointer
    other.m_baseAddress = nullptr;

    // return ref
    return *this;
}

void* MGX::Core::Allocator::FastPoolAllocator::Allocate(size_t size) noexcept
{
    void* returnPointer = nullptr;

    // Allocate slot
    UINT32 slot;
    if (size == m_slotSize && m_baseAddress && (slot = m_ac.AllocateSlot()) != UINT32_MAX)
    {
        // Set output pointer
        returnPointer = (void*)((UINT64)m_baseAddress + (m_slotSize * slot));

        // Debug memset
        #ifdef _DEBUG
        memset(returnPointer, 0xEA, m_slotSize);
        #endif
    }

    return returnPointer;
}

void MGX::Core::Allocator::FastPoolAllocator::Free(void* memory) noexcept
{
    // Compute index of returned slot
    UINT32 returnedSlot = (UINT32)(((UINT64)memory - (UINT64)m_baseAddress) / m_slotSize);

    // Free slot on acceleration
    m_ac.FreeSlot(returnedSlot);
}

UINT64 MGX::Core::Allocator::FastPoolAllocator::GetByteSize() const noexcept
{
    return (UINT64)m_slotSize * m_ac.GetSlotCount();
}

UINT64 MGX::Core::Allocator::FastPoolAllocator::GetByteUsage() const noexcept
{
    return (UINT64)m_slotSize * m_ac.GetSlotUsage();
}
