#include "ModernGX.h"
#include "GPUCommandList.h"

MGX::Core::GPU::CommandList::CommandList(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type) 
{
    // Create command allocator
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateCommandAllocator(...)", ptrDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&m_ptrAllocator)));

    // Create command list
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateCommandList(...)", ptrDevice->CreateCommandList(NULL, type, m_ptrAllocator, nullptr, IID_PPV_ARGS(&m_ptrBase)));

    // On debug clear memory
    #ifdef _DEBUG
    memset(m_resBarriers, 0x00, sizeof(D3D12_RESOURCE_BARRIER) * 256);
    #endif
}

MGX::Core::GPU::CommandList& MGX::Core::GPU::CommandList::operator=(CommandList&& other) noexcept 
{
    // Set this
    m_ptrBase = other.m_ptrBase;
    m_ptrAllocator = other.m_ptrAllocator;
    m_bOpen = other.m_bOpen;

    // Clear other
    other.m_ptrBase.release();
    other.m_ptrAllocator.release();

    // Return ref to this
    return *this;
}

void MGX::Core::GPU::CommandList::Close() noexcept 
{
    if (m_bOpen) 
    {
        // Flush barriers
        ResourceBarrierFlush();

        // Close command list
        m_ptrBase->Close();

        // Set state
        m_bOpen = false;
    }
}

void MGX::Core::GPU::CommandList::Reset() noexcept 
{
    if (!m_bOpen) 
    {
        // Reset
        m_ptrAllocator->Reset();
        m_ptrBase->Reset(m_ptrAllocator, nullptr);

        // Set state
        m_bOpen = true;
    }
}

bool MGX::Core::GPU::CommandList::IsOpen() noexcept 
{
    return m_bOpen;
}

void MGX::Core::GPU::CommandList::ResourceBarrierFlush() noexcept
{
    if (m_barrierUsage)
    {
        // Add to command list
        m_ptrBase->ResourceBarrier(m_barrierUsage, m_resBarriers);

        // On debug clear memory
        #ifdef _DEBUG
        memset(m_resBarriers, 0x00, sizeof(D3D12_RESOURCE_BARRIER) * (m_barrierUsage + 1));
        #endif

        // Reset usage
        m_barrierUsage = 0;
    }
}

HRESULT MGX::Core::GPU::CommandList::name(LPCWSTR name)
{
    HRESULT hr;
    if (FAILED(hr = m_ptrBase.name(name))) return hr;
    if (FAILED(hr = m_ptrAllocator.name(name))) return hr;
    return hr;
}

void MGX::Core::GPU::CommandList::ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtView, FLOAT clearValue[4])
{
    // Flush all barrieres
    ResourceBarrierFlush();

    // Parameter check
    if (!clearValue)
    {
        static FLOAT defaultColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        clearValue = defaultColor;
    }

    // Dispatch call
    m_ptrBase->ClearRenderTargetView(rtView, clearValue, 0, nullptr);
}
