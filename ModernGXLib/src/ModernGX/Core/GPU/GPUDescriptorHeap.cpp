#include "ModernGX.h"
#include "GPUDescriptorHeap.h"

MGX::Core::GPU::DescriptorRange::DescriptorRange(D3D12_GPU_DESCRIPTOR_HANDLE baseGpu, D3D12_CPU_DESCRIPTOR_HANDLE baseCpu, UINT increment, UINT32 range) :
    m_baseAddressGpu(baseGpu),
    m_baseAddressCpu(baseCpu),
    m_rangeSize(range),
    m_incrementStep(increment)
{}

MGX::Core::GPU::DescriptorPair MGX::Core::GPU::DescriptorRange::Get(UINT32 index)
{
    // Debug range check
    #if _DEBUG
    if (index >= m_rangeSize)
    {
        throw std::exception("Trying to access a GpuDescriptor out of range");
    }
    #endif
    
    // Build descritor pair
    return {
        {m_baseAddressGpu.ptr + (m_incrementStep * index)},
        {m_baseAddressCpu.ptr + (m_incrementStep * index)},
    };
}



MGX::Core::GPU::DescriptorHeap::DescriptorHeap(ID3D12Device* ptrDevice, DescriptorHeapUsage usage, UINT32 descriptorCount, IAllocator* ptrCpuAllocator) :
    m_ac(descriptorCount, ptrCpuAllocator)
{
    // Describe heap
    D3D12_DESCRIPTOR_HEAP_DESC dhd = {};
    dhd.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)usage;
    dhd.NumDescriptors = descriptorCount;
    dhd.Flags = 
        (usage != DescriptorHeapUsage::RenderTargetView) && (usage != DescriptorHeapUsage::DepthStencilView) ? 
        D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : 
        D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dhd.NodeMask = NULL;

    // Create descriptor heap
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateDescriptorHeap(...)", ptrDevice->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&m_ptrBase)));

    // Get incrment
    m_descriptorIncrement = ptrDevice->GetDescriptorHandleIncrementSize(dhd.Type);

    // Get base addresses
    if ((usage != DescriptorHeapUsage::RenderTargetView) && (usage != DescriptorHeapUsage::DepthStencilView)) 
    {
        m_baseGpu = m_ptrBase->GetGPUDescriptorHandleForHeapStart();
    }
    m_baseCpu = m_ptrBase->GetCPUDescriptorHandleForHeapStart();

    // Usage
    m_usage = usage;
}

MGX::Core::GPU::DescriptorHeap& MGX::Core::GPU::DescriptorHeap::operator=(DescriptorHeap&& other) noexcept
{
    // Destruct
    this->~DescriptorHeap();

    // Move
    m_ac = std::move(other.m_ac);

    // Copy
    m_usage = other.m_usage;
    m_descriptorIncrement = other.m_descriptorIncrement;
    m_baseGpu = other.m_baseGpu;
    m_baseCpu = other.m_baseCpu;
    m_ptrBase = other.m_ptrBase;

    // Invalidate
    other.m_usage = DescriptorHeapUsage::Null;
    other.m_ptrBase = nullptr;

    return *this;
}

MGX::Core::GPU::DescriptorRange MGX::Core::GPU::DescriptorHeap::Allocate(UINT32 count) noexcept
{
    DescriptorRange range;

    // Alloc slot
    UINT32 slot = m_ac.SlotAlloc(count);
    if (slot != UINT32_MAX)
    {
        // Base offset
        UINT64 offset = m_descriptorIncrement * slot;
        // Build range
        range = DescriptorRange({m_baseGpu.ptr + offset}, {m_baseCpu.ptr + offset}, m_descriptorIncrement, count);
    }

    return range;
}

void MGX::Core::GPU::DescriptorHeap::Free(const DescriptorRange* range) noexcept
{
    // Get slot
    UINT32 slot = (UINT32)((range->m_baseAddressGpu.ptr - m_baseGpu.ptr) / m_descriptorIncrement);

    // Free slot
    m_ac.SlotFree(slot);
}
