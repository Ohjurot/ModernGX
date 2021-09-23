#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Allocator/IAllocator.h>
#include <ModernGX/Core/Allocator/StackMemoryAccelerator.h>

namespace MGX::Core::Allocator
{
    class StackMemoryAllocator : public IMonitoredAllocator
    {
        public:
            // Construct
            StackMemoryAllocator() = default;
            StackMemoryAllocator(const StackMemoryAllocator&) = delete;
            StackMemoryAllocator(UINT32 slotCount, IAllocator* ptrBaseAllocator = StdAllocator::Instance());
            StackMemoryAllocator(UINT32 slotCount, UINT32 slotSize, IAllocator* ptrBaseAllocator = StdAllocator::Instance());

            // Destruct
            ~StackMemoryAllocator();

            // No copy
            StackMemoryAllocator& operator=(const StackMemoryAllocator&) = delete;

            // Move assign
            StackMemoryAllocator& operator=(StackMemoryAllocator&& other) noexcept;

            // Reset function
            void Reset() noexcept;

            // Allocator interface
            void* Allocate(size_t size) noexcept;
            // Asserts that memory is free in reverse order than allocation (STACK)
            void Free(void* memory) noexcept;

            // Monitor interface
            UINT64 GetByteSize() const noexcept;
            UINT64 GetByteUsage() const noexcept;

        private:
            // Base allocator
            IAllocator* m_ptrBaseAllocator = nullptr;

            // Accelerator
            StackMemoryAccelerator m_ac;

            // Slot metric
            UINT32 m_slotSize = 1;
            UINT32 m_slotCount = 0;

            // Usage
            UINT64 m_usage = 0;

            // Slot memory
            void* m_ptrMemory = nullptr;
    };
}
