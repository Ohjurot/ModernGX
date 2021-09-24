#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Allocator/IAllocator.h>
#include <ModernGX/Core/Allocator/FastPoolAccelerator.h>

namespace MGX::Core::Allocator
{
    class ListMemoryAccelerator
    {
        private:
            // Represents a slot in the list
            struct LIST_ALLOCATION_SLOT
            {
                UINT32 size = 0;
                UINT32 nextSlot = UINT32_MAX;
                BOOL used = FALSE;
            };

        public:
            // Construct
            ListMemoryAccelerator() = default;
            ListMemoryAccelerator(const ListMemoryAccelerator&) = delete;
            ListMemoryAccelerator(UINT32 slotCount, IAllocator* ptrBaseAllocator = StdAllocator::Instance());

            // Destruct
            ~ListMemoryAccelerator();

            // Copy
            ListMemoryAccelerator& operator=(const ListMemoryAccelerator&) = delete;

            // Move assign
            ListMemoryAccelerator& operator=(ListMemoryAccelerator&& other) noexcept;

            // Allocation functions
            UINT32 SlotAlloc(UINT32 count = 1) noexcept;
            // Returns allocation size
            UINT32 SlotFree(UINT32 slot) noexcept;

        private:
            // Base allocator
            IAllocator* m_ptrBaseAllocator = nullptr;

            // Pool accelerator for list elements
            FastPoolAccelerator m_listAllocator;

            // List elements
            LIST_ALLOCATION_SLOT* m_ptrListElements = nullptr;
            UINT32 m_listHead = UINT32_MAX;
    };
}
