#include "ModernGX.h"
#include "GPUHeap.h"

MGX::Core::GPU::Heap::Heap(ID3D12Device* ptrDevice, UINT64 size, HeapUsage usage, UINT64 allignment, D3D12_HEAP_FLAGS flags, IAllocator* ptrBaseAllocator)
{
    // Heap accelerator
    UINT32 slotCount = (UINT32)( (size + D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT - 1) / D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
    m_ac = std::move(Allocator::ListMemoryAccelerator(slotCount, ptrBaseAllocator));

    // Unmask non resident
    if (flags & D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT)
    {
        flags &= ~D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT;
#ifdef _DEBUG
        OutputDebugString(L"[MGX Error] Specifing the heap flag D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT is not valid. Flag was cleared!\n");
#endif
    }

    // Describe the heap
    D3D12_HEAP_DESC hd = {};
    hd.SizeInBytes = size;
    hd.Flags = flags;
    hd.Alignment = allignment;
    hd.Properties.Type = (D3D12_HEAP_TYPE)usage;
    hd.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    hd.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    hd.Properties.CreationNodeMask = NULL;
    hd.Properties.VisibleNodeMask = NULL;

    // Create heap
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateHeap(...)", ptrDevice->CreateHeap(&hd, IID_PPV_ARGS(&m_ptrHeap)));
}

MGX::Core::GPU::Heap& MGX::Core::GPU::Heap::operator=(Heap&& other) noexcept
{
    // Destruct this
    this->~Heap();

    // Move 
    m_ac = std::move(other.m_ac);

    // Copy
    m_ptrHeap = other.m_ptrHeap;

    // Invalidate
    other.m_ptrHeap = nullptr;

    return *this;
}

bool MGX::Core::GPU::Heap::Allocate(HeapAllocationCookie* ptrCookie) noexcept
{
    // Get slot count
    UINT32 slotCount = (UINT32)( (ptrCookie->size + D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT - 1) / D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT );

    // Try alloc
    UINT32 slotOffset = m_ac.SlotAlloc(slotCount);
    if (slotOffset != UINT32_MAX)
    {
        // Set other members
        ptrCookie->offset = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * slotOffset;
        ptrCookie->ptrHeap = m_ptrHeap;

        // Succeeded
        return true;
    }

    // Allocation failed
    return false;
}

void MGX::Core::GPU::Heap::Free(const HeapAllocationCookie* ptrCookie) noexcept
{
    // Get slot offset
    UINT32 slotOffset = (UINT32)(ptrCookie->offset / D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

    // Free slot
    m_ac.SlotFree(slotOffset);
}
