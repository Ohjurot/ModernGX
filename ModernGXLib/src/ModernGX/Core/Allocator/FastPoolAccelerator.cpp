#include "ModernGX.h"
#include "FastPoolAccelerator.h"

MGX::Core::Allocator::FastPoolAccelerator::FastPoolAccelerator(UINT32 slotCount, IAllocator* ptrBaseAllocator) :
    m_ptrBaseAllocator(ptrBaseAllocator),
    m_slotCount(slotCount)
{
    // Allocate slot memory
    m_ptrSlots = (POOL_ALLOCATION_SLOT*)m_ptrBaseAllocator->Allocate(sizeof(POOL_ALLOCATION_SLOT) * slotCount);

    // Check if valid
    if (m_ptrSlots)
    {
        // Fill slots
        for (UINT32 i = 0; i < m_slotCount - 1; i++)
        {
            m_ptrSlots[i].nextFreeSlot = i + 1;
        }
        m_ptrSlots[m_slotCount - 1].nextFreeSlot = UINT32_MAX;

        // Set last free slot
        m_lastFreeSlot = m_slotCount - 1;
    }
    else
    {
        // Invalid count
        m_slotCount = 0;
    }
}

MGX::Core::Allocator::FastPoolAccelerator::~FastPoolAccelerator()
{
    // Free memory if valid
    if (m_ptrSlots)
        m_ptrBaseAllocator->Free(m_ptrSlots);
}

MGX::Core::Allocator::FastPoolAccelerator& MGX::Core::Allocator::FastPoolAccelerator::operator=(FastPoolAccelerator&& other) noexcept
{
    // Shall only work on invalid object
    if (!m_slotCount)
    {
        // Memcpy to this
        memcpy(this, &other, sizeof(FastPoolAccelerator));

        // unset others pointers (destruction of other must not free memory)
        other.m_ptrSlots = nullptr;
        // unset others slot count (make allocator not usable)
        other.m_slotCount = 0;
    }

    // Return
    return *this;
}

UINT32 MGX::Core::Allocator::FastPoolAccelerator::AllocateSlot() noexcept
{
    // Store next slot
    UINT32 next = m_nextFreeSlot;

    // Set next free slot if possible
    if (m_ptrSlots && next != UINT32_MAX) {
        m_nextFreeSlot = m_ptrSlots[m_nextFreeSlot].nextFreeSlot;

        // Allocation succeded
        m_slotUsage++;
    }

    return next;
}

void MGX::Core::Allocator::FastPoolAccelerator::FreeSlot(UINT32 slot) noexcept
{
    // Is there a former free slot
    if (m_lastFreeSlot != UINT32_MAX)
    {
        // Put element at end of "liked list"
        m_ptrSlots[m_lastFreeSlot].nextFreeSlot = slot;
        m_ptrSlots[slot].nextFreeSlot = UINT32_MAX;

        // Set the last slot
        m_lastFreeSlot = slot;

        // Set next free slot if required
        if (m_nextFreeSlot == UINT32_MAX)
        {
            m_nextFreeSlot = slot;
        }

        // Freeing succeded
        m_slotUsage--;
    }
}
