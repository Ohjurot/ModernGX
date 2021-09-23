#pragma once
#include <ModernGX.h>
#include <ModernGX/Core/Allocator/IAllocator.h>
#include <ModernGX/Core/Allocator/ListMemoryAccelerator.h>

namespace MGX::Core::Allocator
{
    // Any size memory allocator
    class MemoryAllocator : public IMonitoredAllocator
    {
        public:
            // Construct
            MemoryAllocator() = default;
            MemoryAllocator(const MemoryAllocator&) = delete;
            MemoryAllocator(UINT32 slotCount, UINT slotSize = 64, IAllocator* ptrBaseAllocator = StdAllocator::Instance());

            // Destruct
            ~MemoryAllocator();

            // Copy
            MemoryAllocator& operator=(const MemoryAllocator&) = delete;

            // Move
            MemoryAllocator& operator=(MemoryAllocator&& other) noexcept;

            // Allocator Interface
            void* Allocate(size_t size) noexcept override;
            void Free(void* memory) noexcept override;

            // Monitor Interface
            UINT64 GetByteSize() const noexcept override;
            UINT64 GetByteUsage() const noexcept override;
        private:
            // Parent allocator
            IAllocator* m_ptrBaseAllocator = nullptr;

            // Memory block
            void* m_ptrMemory = nullptr;

            // Slot allocation accelerator
            ListMemoryAccelerator m_ac;

            // Slot metrics
            UINT32 m_slotSize = 0;
            UINT32 m_slotCount = 0;

            // Usage metrics
            UINT64 m_allocatorUsage = 0;
    };
}
