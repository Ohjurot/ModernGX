#include "ModernGX.h"
#include "GPUCommandList.h"

MGX::Core::GPU::CommandList::CommandList(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type)  :
    m_type(type)
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
    // Release this
    this->~CommandList();

    // Set this
    m_ptrBase = other.m_ptrBase;
    m_ptrAllocator = other.m_ptrAllocator;
    m_bOpen = other.m_bOpen;
    m_type = other.m_type;

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

void MGX::Core::GPU::CommandList::RSSetViewportAndRect(D3D12_VIEWPORT* viewPort, D3D12_RECT* rect)
{
    // Set view port and / or rect
    if (viewPort)
        m_ptrBase->RSSetViewports(1, viewPort);
    if (rect)
        m_ptrBase->RSSetScissorRects(1, rect);
}

void MGX::Core::GPU::CommandList::RSSetViewportAndRect(unsigned int count ...)
{
    // Target lists
    D3D12_VIEWPORT vps[8];
    RECT rects[8];

    // Varlist loop
    va_list vlist;
    va_start(vlist, count);
    for (unsigned int i = 0; i < count; i++)
    {
        // Extract to array
        vps[i] = va_arg(vlist, D3D12_VIEWPORT);
        rects[i] = va_arg(vlist, RECT);
    }

    // End list
    va_end(vlist);

    // Set on command list
    m_ptrBase->RSSetViewports(count, vps);
    m_ptrBase->RSSetScissorRects(count, rects);
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

void MGX::Core::GPU::CommandList::ClearDepthStencil(D3D12_CPU_DESCRIPTOR_HANDLE dsView, FLOAT depth, UINT8 stencil)
{
    // Flush and clear
    ResourceBarrierFlush();
    m_ptrBase->ClearDepthStencilView(dsView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
}

void MGX::Core::GPU::CommandList::OMSetRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtView)
{
    m_ptrBase->OMSetRenderTargets(1, &rtView, TRUE, nullptr);
}

void MGX::Core::GPU::CommandList::OMSetRenderTargetViews(D3D12_CPU_DESCRIPTOR_HANDLE dsView, D3D12_CPU_DESCRIPTOR_HANDLE rtView)
{
    m_ptrBase->OMSetRenderTargets(1, &rtView, TRUE, dsView.ptr ? &dsView : nullptr);
}

void MGX::Core::GPU::CommandList::IASetBuffer(D3D12_VERTEX_BUFFER_VIEW* vbView, D3D12_PRIMITIVE_TOPOLOGY topology)
{
    m_ptrBase->IASetPrimitiveTopology(topology);
    m_ptrBase->IASetVertexBuffers(0, 1, vbView);
}

void MGX::Core::GPU::CommandList::IASetBuffer(D3D12_VERTEX_BUFFER_VIEW* vbView, D3D12_INDEX_BUFFER_VIEW* ibView, D3D12_PRIMITIVE_TOPOLOGY topology)
{
    m_ptrBase->IASetPrimitiveTopology(topology);
    m_ptrBase->IASetVertexBuffers(0, 1, vbView);
    m_ptrBase->IASetIndexBuffer(ibView);
}
