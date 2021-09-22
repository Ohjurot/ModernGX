#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Allocator/IAllocator.h>

namespace MGX::Core::Allocator
{
    // Stack accelerator struct
    class StackMemoryAccelerator
    {
        public:
            // Construct
            StackMemoryAccelerator() = default;
            StackMemoryAccelerator(const StackMemoryAccelerator&) = delete;
            StackMemoryAccelerator(UINT32 size);

            // Default destruct
            ~StackMemoryAccelerator() = default;

            // No copy
            StackMemoryAccelerator operator=(const StackMemoryAccelerator&) = delete;

            // Move assign
            StackMemoryAccelerator& operator=(StackMemoryAccelerator&& other) noexcept;

            // Bool check valid
            operator bool() const noexcept;

            // Assign
            UINT32 SlotAlloc(UINT32 size = 1) noexcept;
            // Reset
            void Reset() noexcept;

        private:
            // Count and head position
            UINT32 m_count = 0;
            UINT32 m_head = 0;
    };
}
