#include "ModernGX.h"
#include "GPUResource.h"

MGX::Core::GPU::Resource::Resource(ID3D12Resource* ptrResource, D3D12_RESOURCE_STATES initialState) noexcept :
    m_currentState(initialState)
{
    m_ptrBase = ptrResource;
}

MGX::Core::GPU::Resource::Resource(ID3D12Device* ptrDevice, HeapUsage usage, const D3D12_RESOURCE_DESC* ptrDesc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* ptrClearValue, D3D12_HEAP_FLAGS heapFlags)
{
    // Describe heap
    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = (D3D12_HEAP_TYPE)usage;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = NULL;
    prop.VisibleNodeMask = NULL;

    // Create committed resource
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateCommittedResource(...)", ptrDevice->CreateCommittedResource(&prop, heapFlags, ptrDesc, initialState, ptrClearValue, IID_PPV_ARGS(&m_ptrBase)));
}

MGX::Core::GPU::Resource::Resource(ID3D12Device* ptrDevice, const D3D12_RESOURCE_DESC* ptrDesc, HeapAllocationCookie allocationCookie, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* ptrClearValue)
{
    MGX_EVALUATE_HRESULT("ID3D12Device->CreatePlacedResource(...)", ptrDevice->CreatePlacedResource(allocationCookie.ptrHeap, allocationCookie.offset, ptrDesc, initialState, ptrClearValue, IID_PPV_ARGS(&m_ptrBase)));
}

MGX::Core::GPU::Resource& MGX::Core::GPU::Resource::operator=(Resource&& other) noexcept
{
    // Destruct this
    this->~Resource();

    // Copy
    m_ptrBase = other.m_ptrBase;
    m_currentState = other.m_currentState;

    // Invalidate
    other.m_ptrBase = nullptr;

    return *this;
}

bool MGX::Core::GPU::Resource::SetResourceState(D3D12_RESOURCE_STATES newState, D3D12_RESOURCE_BARRIER* ptrBarrier) noexcept
{
    // Check if transistion is required
    if (m_currentState != newState)
    {
        // Fill barrier
        ptrBarrier->Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        ptrBarrier->Transition.pResource = m_ptrBase;
        ptrBarrier->Transition.StateBefore = m_currentState;
        ptrBarrier->Transition.StateAfter = newState;
        ptrBarrier->Transition.Subresource = 0;
        ptrBarrier->Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        // Update inernal state
        m_currentState = newState;
        return true;
    }
    
    // Fallback
    return false;
}
