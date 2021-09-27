#pragma once

#include <ModernGX.h>

namespace MGX::Core::GPU
{
    // Heap allocation cookie
    struct HeapAllocationCookie
    {
        ID3D12Heap* ptrHeap = nullptr;
        UINT64 offset = UINT64_MAX;
        UINT64 size = 0;

        // Boolean operator
        operator bool() const noexcept
        {
            return (ptrHeap != nullptr) && (size) && (offset != UINT64_MAX);
        }
    };
}
