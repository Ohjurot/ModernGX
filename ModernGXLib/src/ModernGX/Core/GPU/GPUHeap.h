#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/Allocator/ListMemoryAccelerator.h>
#include <ModernGX/Core/GPU/GPUHeapAllocationCookie.h>

namespace MGX::Core::GPU 
{
    // Heap location
    enum class HeapUsage
    {
        // Default heap (GPU Work Memory)
        Default = D3D12_HEAP_TYPE_DEFAULT,

        // Upload heap (CPU Write --> GPU Read)
        Upload = D3D12_HEAP_TYPE_UPLOAD,

        // Download heap (GPU Write --> CPU Read)
        Download = D3D12_HEAP_TYPE_READBACK,
    };

    // GPU Memory heap
    class Heap
    {
        public:
            // Construct
            Heap() noexcept = default;
            Heap(const Heap&) noexcept = default;
            Heap(ID3D12Device* ptrDevice, UINT64 size, HeapUsage usage = HeapUsage::Default, UINT64 allignment = 0, D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE, IAllocator* ptrBaseAllocator = StdAllocator::Instance());

            // Copy
            Heap& operator=(const Heap&) = delete;

            // Move
            Heap& operator=(Heap&& other) noexcept;

            // Allocate cookie on heap
            bool Allocate(HeapAllocationCookie* ptrCookie) noexcept;
            // Free cookie on heap
            void Free(const HeapAllocationCookie* ptrCookie) noexcept;

        private:
            // Heap accelerator
            Allocator::ListMemoryAccelerator m_ac;

            // DirectX Heap object
            ComPointer<ID3D12Heap> m_ptrHeap;
    };
}
