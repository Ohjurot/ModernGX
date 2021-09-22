#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Allocator/IAllocator.h>

namespace MGX::Core::Allocator
{
    // Accelerator for pool allocators
    class FastPoolAccelerator
    {
        private:
            // Allocator acceleration struct
            struct POOL_ALLOCATION_SLOT
            {
                // Next free slot index (UINT32_MAX = no next free slot)
                UINT32 nextFreeSlot = UINT32_MAX;
            };

        public:
            // Construct
            FastPoolAccelerator() = default;
            FastPoolAccelerator(const FastPoolAccelerator&) = delete;
            FastPoolAccelerator(UINT32 slotCount, IAllocator* ptrBaseAllocator = StdAllocator::Instance());

            // Destruct
            ~FastPoolAccelerator();

            // Move assign
            FastPoolAccelerator& operator=(FastPoolAccelerator&& other) noexcept;

            // Allocate / Free slot
            UINT32 AllocateSlot() noexcept;
            void FreeSlot(UINT32 slot) noexcept;

            // Size / Occupacity getters
            inline UINT32 GetSlotCount() const noexcept {
                return m_slotCount;
            }
            inline UINT32 GetSlotUsage() const noexcept {
                return m_slotUsage;
            }

        private:
            // Base allocator
            IAllocator* m_ptrBaseAllocator = nullptr;

            // Slot count and usage
            UINT32 m_slotCount = 0;
            UINT32 m_slotUsage = 0;

            // Slots itself
            UINT32 m_nextFreeSlot = 0;
            UINT32 m_lastFreeSlot = UINT32_MAX;

            // Acceleration struct
            POOL_ALLOCATION_SLOT* m_ptrSlots = nullptr;
    };
}
