#pragma once

#include <ModernGX.h>

namespace MGX::Core::GPU
{
    // Heap allocation cookie
    struct HeapAllocationCookie
    {
        ID3D12Heap* ptrHeap = nullptr;
        UINT64 offset = 0;
        UINT64 size = 0;
    };
}
