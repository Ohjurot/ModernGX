#include "ModernGX.h"
#include "GPUUploadStack.h"

MGX::Helpers::GPUUploadStack::GPUUploadStack(ID3D12Device* ptrDevice, UINT64 size) :
    m_sharedBuffer(ptrDevice, Core::GPU::HeapUsage::Upload, size),
    m_size(size)
{

}

MGX::Helpers::GPUUploadStack::GPUUploadStack(ID3D12Device* ptrDevice, Core::GPU::HeapAllocationCookie heapAllocation, UINT64 size) :
    m_sharedBuffer(ptrDevice, heapAllocation, size),
    m_size(size)
{

}

MGX::Helpers::GPUUploadStack& MGX::Helpers::GPUUploadStack::operator=(GPUUploadStack&& other) noexcept
{
    // Destruct
    this->~GPUUploadStack();

    // Copy / Move
    m_head = other.m_head;
    m_size = other.m_size;
    m_ptrCurrentCommandList = other.m_ptrCurrentCommandList;
    m_mappedBuffer = std::move(other.m_mappedBuffer);
    m_sharedBuffer = std::move(other.m_sharedBuffer);

    // Unset other
    other.m_ptrCurrentCommandList = nullptr;
    other.m_size = 0;

    return *this;
}

bool MGX::Helpers::GPUUploadStack::Reset(Core::GPU::CommandList* ptrCmdList)
{
    // Reset head
    m_head = 0;
    
    // Set command list
    if (ptrCmdList)
    {
        // Store
        m_ptrCurrentCommandList = ptrCmdList;

        // Resource barrier
        if (m_sharedBuffer.SetResourceState(D3D12_RESOURCE_STATE_COPY_SOURCE, ptrCmdList->ResourceBarrierPeek()))
            ptrCmdList->ResourceBarrierPush();
    }

    // Map buffer
    m_mappedBuffer = m_sharedBuffer.Map();

    return m_ptrCurrentCommandList != nullptr;
}

void MGX::Helpers::GPUUploadStack::Close()
{
    // Unmap
    if(m_mappedBuffer)
        m_mappedBuffer.Unmap();
}

bool MGX::Helpers::GPUUploadStack::CopyBuffer(Core::GType::Buffer* ptrDest, const void* srcData, UINT64 size, UINT64 destOffset)
{
    // Check and copy
    bool canCopy = m_head + size <= m_size && destOffset + size <= ptrDest->Size() && m_mappedBuffer;
    if (canCopy)
    {
        // Set can copy to actual d3d copy
        canCopy = ptrDest->CopyFrom(m_ptrCurrentCommandList, &m_sharedBuffer, size, destOffset, m_head);
        if (canCopy)
        {
            // Copy and increment size
            memcpy(&m_mappedBuffer[m_head], srcData, size);
            m_head += size;
        }
    }

    return canCopy;
}

bool MGX::Helpers::GPUUploadStack::CopyTexture(Core::GType::Texture* ptrDstTexture, Coding::TextureFile* ptrSrcTexture)
{
    // Allign head
    if (m_head % D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT)
        m_head += D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT - (m_head % D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

    // Compute texture bounds
    auto textureStride = ((ptrSrcTexture->GetBPP() + 7) / 8) * ptrSrcTexture->GetWidth();
    auto textureSize = textureStride * ptrSrcTexture->GetHeight();

    // Check if copy is possible
    bool canCopy = m_head + textureSize <= m_size && m_mappedBuffer;
    if (canCopy)
    {
        // DirectX copy
        canCopy = ptrDstTexture->CopyFromBuffer(m_ptrCurrentCommandList, &m_sharedBuffer, m_head, true);
        if (canCopy)
        {
            ptrSrcTexture->ReadPixels(0, 0, ptrSrcTexture->GetWidth(), ptrSrcTexture->GetHeight(), &m_mappedBuffer[m_head], textureSize, textureStride, ptrDstTexture->GetFormat()); // Dont care
            m_head += textureSize;
        }
    }

    return canCopy;
}

