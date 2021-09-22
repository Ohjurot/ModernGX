#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>

namespace MGX::Core::GPU 
{
    // Heap location
    enum class HeapUsage 
    {
        // Default heap (GPU Work Memory)
        Default,

        // Upload heap (CPU Write --> GPU Read)
        Upload,

        // Download heap (GPU Write --> CPU Read)
        Download,
    };
    
    // GPU Memory heap
    class Heap : public Foundation::COMGetable<ID3D12Heap> 
    {
        public:
            // Construct
            Heap() noexcept = default;
            Heap(const Heap&) noexcept = default;
            Heap(ID3D12Device* ptrDevice, UINT64 size, HeapUsage usage = HeapUsage::Default, UINT64 allignment = 0, D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE);
    };
}
