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
    // Destruct this
    this->~ListMemoryAccelerator();

    // Parent move
    m_listAllocator = std::move(other.m_listAllocator);
        
    // Copy
    m_ptrBaseAllocator = other.m_ptrBaseAllocator;
    m_ptrListElements = other.m_ptrListElements;
    m_listHead = other.m_listHead;

    // Clear other
    other.m_ptrListElements = nullptr;
    other.m_listHead = UINT32_MAX;
    
    return *this;
}

UINT32 MGX::Core::Allocator::ListMemoryAccelerator::SlotAlloc(UINT32 count) noexcept
{
    UINT32 slot = UINT32_MAX;

    // Traverse list
    UINT32 currentSlot = m_listHead;
    UINT32 offset = 0;
    while (currentSlot != UINT32_MAX)
    {
        // List element
        LIST_ALLOCATION_SLOT* const ptrSlot = &m_ptrListElements[currentSlot];

        // Check if slot size fits and is free
        if (!ptrSlot->used && ptrSlot->size >= count)
        {
            // Set slot
            slot = offset;

            // Mark slot as used
            ptrSlot->used = TRUE;

            // Check if list need to be expanded
            if (ptrSlot->size != count)
            {
                // Store old next
                const UINT32 oldNext = ptrSlot->nextSlot;

                // Allocate new next
                ptrSlot->nextSlot = m_listAllocator.AllocateSlot();
                if (ptrSlot->nextSlot != UINT32_MAX)
                {
                    // Get slot pointer
                    LIST_ALLOCATION_SLOT* const ptrNextSlot = &m_ptrListElements[ptrSlot->nextSlot];

                    // New Slot Data
                    *ptrNextSlot = { ptrSlot->size - count, oldNext, FALSE };

                    // Old slot size
                    ptrSlot->size = count;
                }
                else
                {
                    // Fallback: more memory used
                    ptrSlot->nextSlot = oldNext;
                }
            }

            // Stop loop
            break;
        }

        // Increment offset
        offset += ptrSlot->size;
        // Next slot
        currentSlot = ptrSlot->nextSlot;
    }

    return slot;
}

UINT32 MGX::Core::Allocator::ListMemoryAccelerator::SlotFree(UINT32 slot) noexcept
{
    // Output size
    UINT32 oSize = 0;

    // Find slot
    UINT32 prevSlot = UINT32_MAX;
    UINT32 currentSlot = m_listHead;
    UINT32 offset = 0;
    while (currentSlot != UINT32_MAX)
    {
        // Get slot pointer
        LIST_ALLOCATION_SLOT* const ptrSlot = &m_ptrListElements[currentSlot];

        // Check if offset is equal
        if (offset == slot)
        {
            // Mark current slot as free
            ptrSlot->used = FALSE;
            oSize = ptrSlot->size;

            // Check if collaps from right
            if (ptrSlot->nextSlot != UINT32_MAX)
            {
                // Get and check prev slot
                UINT32 oldNextSlot = ptrSlot->nextSlot;
                LIST_ALLOCATION_SLOT* const ptrNextSlot = &m_ptrListElements[oldNextSlot];
                if (!ptrNextSlot->used)
                {
                    // Sum sizes
                    ptrSlot->size += ptrNextSlot->size;

                    // Link over next slot
                    ptrSlot->nextSlot = ptrNextSlot->nextSlot;

                    // Free next slot
                    m_listAllocator.FreeSlot(oldNextSlot);
                }
            }

            // Check if collaps to left
            if (prevSlot != UINT32_MAX)
            {
                // Get and check prev slot
                LIST_ALLOCATION_SLOT* const ptrPrvSlot = &m_ptrListElements[prevSlot];
                if (!ptrPrvSlot->used)
                {
                    // Sum sizes
                    ptrPrvSlot->size += ptrSlot->size;

                    // Link over current
                    ptrPrvSlot->nextSlot = ptrSlot->nextSlot;

                    // Free current slot
                    m_listAllocator.FreeSlot(currentSlot);
                }
            }

            // End free
            break;
        }

        // Increment offset
        offset += ptrSlot->size;

        // Next slot
        prevSlot = currentSlot;
        currentSlot = ptrSlot->nextSlot;
    }

    return oSize;
}
