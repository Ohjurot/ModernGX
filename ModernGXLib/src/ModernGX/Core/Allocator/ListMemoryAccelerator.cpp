#include "ModernGX.h"
#include "ListMemoryAccelerator.h"

MGX::Core::Allocator::ListMemoryAccelerator::ListMemoryAccelerator(UINT32 slotCount, IAllocator* ptrBaseAllocator) :
    m_ptrBaseAllocator(ptrBaseAllocator),
    m_listAllocator(slotCount, ptrBaseAllocator)
{
    // Allocate acceleraion slot
    m_ptrListElements = m_ptrBaseAllocator->TAllocate<LIST_ALLOCATION_SLOT>(slotCount);
    // Set first element
    if (m_ptrListElements)
    {
        // Allocate pool slot
        m_listHead = m_listAllocator.AllocateSlot();
        if (m_listHead != UINT32_MAX)
        {
            // Set element
            m_ptrListElements[m_listHead] = {slotCount, UINT32_MAX, FALSE};
        }
    }
}

MGX::Core::Allocator::ListMemoryAccelerator::~ListMemoryAccelerator()
{
    // Free memmory
    if (m_ptrListElements)
    {
        m_ptrBaseAllocator->Free(m_ptrListElements);
    }
}

MGX::Core::Allocator::ListMemoryAccelerator& MGX::Core::Allocator::ListMemoryAccelerator::operator=(ListMemoryAccelerator&& other) noexcept
{
    // Only if this is not valid
    if (!m_ptrListElements)
    {
        // Parent move
        m_listAllocator = std::move(other.m_listAllocator);
        
        // Copy
        m_ptrBaseAllocator = other.m_ptrBaseAllocator;
        m_ptrListElements = other.m_ptrListElements;
        m_listHead = other.m_listHead;

        // Clear other
        other.m_ptrListElements = nullptr;
        other.m_listHead = UINT32_MAX;
    }

    return *this;
}
