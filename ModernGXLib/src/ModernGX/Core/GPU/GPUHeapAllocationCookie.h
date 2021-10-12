#pragma once

#include <ModernGX.h>

namespace MGX::Core::GPU
{
    // Heap usage
    enum class HeapUsage
    {
        // Default heap (GPU Work Memory)
        Default = D3D12_HEAP_TYPE_DEFAULT,

        // Upload heap (CPU Write --> GPU Read)
        Upload = D3D12_HEAP_TYPE_UPLOAD,

        // Download heap (GPU Write --> CPU Read)
        Download = D3D12_HEAP_TYPE_READBACK,
    };

    // Heap allocation cookie
    struct HeapAllocationCookie
    {
        ID3D12Heap* ptrHeap = nullptr;
        UINT64 offset = UINT64_MAX;
        UINT64 size = 0;
        HeapUsage usage = HeapUsage::Default;

        // Boolean operator
        operator bool() const noexcept
        {
            return (ptrHeap != nullptr) && (size) && (offset != UINT64_MAX);
        }
    };
}
