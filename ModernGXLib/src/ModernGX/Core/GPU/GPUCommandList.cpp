#include "ModernGX.h"
#include "GPUCommandList.h"

MGX::Core::GPU::CommandList::CommandList(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type) {
    // Create command allocator
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateCommandAllocator(...)", ptrDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&m_ptrAllocator)));

    // Create command list
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateCommandList(...)", ptrDevice->CreateCommandList(NULL, type, m_ptrAllocator, nullptr, IID_PPV_ARGS(&m_ptrBase)));
}

void MGX::Core::GPU::CommandList::Close() noexcept {
    if (m_bOpen) {
        // Close command list
        m_ptrBase->Close();

        // Set state
        m_bOpen = false;
    }
}

void MGX::Core::GPU::CommandList::Reset() noexcept {
    if (!m_bOpen) {
        // Reset
        m_ptrAllocator->Reset();
        m_ptrBase->Reset(m_ptrAllocator, nullptr);

        // Set state
        m_bOpen = true;

    }
}

bool MGX::Core::GPU::CommandList::IsOpen() noexcept {
    return m_bOpen;
}
