#include "ModernGX.h"
#include "GPUHeap.h"

MGX::Core::GPU::Heap::Heap(ID3D12Device* ptrDevice, UINT64 size, HeapUsage usage, UINT64 allignment, D3D12_HEAP_FLAGS flags) 
{
    // Unmask non resident
    if (flags & D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT) 
    {
        flags &= ~D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT;
        #ifdef _DEBUG
        OutputDebugString(L"[MGX Error] Specifing the heap flag D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT is not valid. Flag was cleared!\n");
        #endif
    }
}
