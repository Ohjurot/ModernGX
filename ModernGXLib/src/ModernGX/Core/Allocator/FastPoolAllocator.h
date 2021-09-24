#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Allocator/IAllocator.h>
#include <ModernGX/Core/Allocator/FastPoolAccelerator.h>

namespace MGX::Core::Allocator 
{
    // Speed optimized pool allocator
    class FastPoolAllocator : public IMonitoredAllocator
    {
        public:
            // Construct
            FastPoolAllocator() = default;
            FastPoolAllocator(const FastPoolAllocator&) = delete;
            FastPoolAllocator(UINT32 slotCount, UINT32 slotSize, IAllocator* ptrBaseAllocator = StdAllocator::Instance());

            // Destruct
            ~FastPoolAllocator();

            // Copy
            FastPoolAllocator& operator=(const FastPoolAllocator&) = delete;

            // Move assign
            FastPoolAllocator& operator=(FastPoolAllocator&& other) noexcept;

            // Allocate memory (IAllocator)
            void* Allocate(size_t size) noexcept override;
            void Free(void* memory) noexcept override;
            // Get Metrics (IMonitoredAllocator)
            UINT64 GetByteSize() const noexcept override;
            UINT64 GetByteUsage() const noexcept override;

        private:
            // Base allocator
            IAllocator* m_ptrBaseAllocator = nullptr;

            // Base addressing
            void* m_baseAddress = 0;
            UINT64 m_slotSize = 0;

            // Accelerator
            FastPoolAccelerator m_ac;
    };
}
