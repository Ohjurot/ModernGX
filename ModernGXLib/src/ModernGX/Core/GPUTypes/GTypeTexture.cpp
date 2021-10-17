#include "ModernGX.h"
#include "GTypeTexture.h"

MGX::Core::GType::Texture::Texture(ID3D12Device* ptrDevice, GPU::HeapUsage usage, DXGI_FORMAT textureFormat, UINT width, UINT height, UINT16 depth, INT mipLevels, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_FLAGS flags, D3D12_TEXTURE_LAYOUT layout, D3D12_HEAP_FLAGS heapFlags) :
    m_width(width ? width : 1), m_height(height ? height : 1), m_depth(depth ? depth : 1),
    m_mipCount(mipLevels), 
    m_format(textureFormat),
    m_bytesPerPixel(Util::DXGIFormat::GetBytedPerPixel(textureFormat))
{
    // Desiption
    D3D12_RESOURCE_DESC rd = this->CreateResourceDesc(textureFormat, width, height, depth, mipLevels, sampleCount, sampleQuality, layout, flags);

    // Set member
    if (depth > 1)
        m_dimension = TextureDimension::Texture3D;
    else if (width > 1)
        m_dimension = TextureDimension::Texture2D;
    else
        m_dimension = TextureDimension::Texture1D;

    // Super construct
    this->GPU::Resource::Resource(ptrDevice, usage, &rd);
}

MGX::Core::GType::Texture::Texture(ID3D12Device* ptrDevice, GPU::HeapAllocationCookie allocationCookie, DXGI_FORMAT textureFormat, UINT width, UINT height, UINT16 depth, INT mipLevels, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_FLAGS flags, D3D12_TEXTURE_LAYOUT layout) :
    m_width(width ? width : 1), m_height(height ? height : 1), m_depth(depth ? depth : 1),
    m_mipCount(mipLevels), 
    m_format(textureFormat),
    m_bytesPerPixel(Util::DXGIFormat::GetBytedPerPixel(textureFormat))
{
    // Desiption
    D3D12_RESOURCE_DESC rd = this->CreateResourceDesc(textureFormat, width, height, depth, mipLevels, sampleCount, sampleQuality, layout, flags);

    // Set member
    if (depth > 1)
        m_dimension = TextureDimension::Texture3D;
    else if (width > 1)
        m_dimension = TextureDimension::Texture2D;
    else
        m_dimension = TextureDimension::Texture1D;

    // Super construct
    this->GPU::Resource::Resource(ptrDevice, &rd, allocationCookie);
}

MGX::Core::GType::Texture::Texture(ID3D12Device* ptrDevice, DXGI_FORMAT textureFormat, UINT width, UINT height, UINT16 depth, INT mipLevels, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_FLAGS flags, D3D12_TEXTURE_LAYOUT layout) :
    m_width(width ? width : 1), m_height(height ? height : 1), m_depth(depth ? depth : 1),
    m_mipCount(mipLevels),
    m_format(textureFormat),
    m_bytesPerPixel(Util::DXGIFormat::GetBytedPerPixel(textureFormat))
{
    // Desiption
    D3D12_RESOURCE_DESC rd = this->CreateResourceDesc(textureFormat, width, height, depth, mipLevels, sampleCount, sampleQuality, layout, flags);

    // Set member
    if (depth > 1)
        m_dimension = TextureDimension::Texture3D;
    else if (width > 1)
        m_dimension = TextureDimension::Texture2D;
    else
        m_dimension = TextureDimension::Texture1D;

    // Super construct
    this->GPU::Resource::Resource(ptrDevice, &rd);
}

MGX::Core::GType::Texture& MGX::Core::GType::Texture::operator=(Texture&& other) noexcept
{
    // Destruct this
    this->~Texture();

    // copy from other
    m_width = other.m_width;
    m_height = other.m_height;
    m_depth = other.m_depth;
    m_format = other.m_format;
    m_bytesPerPixel = other.m_bytesPerPixel;
    m_mipCount = other.m_mipCount;
    m_ptrBase = other.m_ptrBase;

    // Delete other
    other.m_width = 0;
    other.m_height = 0;
    other.m_depth = 0;
    other.m_format = DXGI_FORMAT_UNKNOWN;
    m_bytesPerPixel = 0;
    other.m_mipCount = 0;
    other.m_ptrBase = nullptr;

    return* this;
}

bool MGX::Core::GType::Texture::CopyFrom(GPU::CommandList* ptrCmdList, Texture* ptrSrc, bool allowBarriers)
{
    // Check if copy is possible
    bool canCopy = ptrSrc->m_dimension == m_dimension && ptrSrc->m_width == m_width && ptrSrc->m_height == m_height 
        && ptrSrc->m_depth == m_depth && ptrSrc->m_mipCount == m_mipCount && CheckSetResourceBarriers(ptrCmdList, ptrSrc, this, allowBarriers);;
    // Copy
    if (canCopy)
    {
        // Copy
        ptrCmdList->Ptr()->CopyResource(m_ptrBase, ptrSrc->Ptr());
    }

    return canCopy;
}

bool MGX::Core::GType::Texture::CopyTo(GPU::CommandList* ptrCmdList, Texture* ptrDest, bool allowBarriers)
{
    // Check if copy is possible
    bool canCopy = ptrDest->m_dimension == m_dimension && ptrDest->m_width == m_width && ptrDest->m_height == m_height
        && ptrDest->m_depth == m_depth && ptrDest->m_mipCount == m_mipCount && CheckSetResourceBarriers(ptrCmdList, this, ptrDest, allowBarriers);

    // Copy
    if (canCopy)
    {
        // Copy
        ptrCmdList->Ptr()->CopyResource(ptrDest->Ptr(), m_ptrBase);
    }

    return canCopy;
}

bool MGX::Core::GType::Texture::CopyFromBuffer(GPU::CommandList* ptrCmdList, GPU::Resource* ptrSrc, UINT bufferOffset, bool allowBarries)
{
    // Vertify size
    bool canCopy = CheckSetResourceBarriers(ptrCmdList, ptrSrc, this, allowBarries);

    // Copy
    if (canCopy)
    {
        // Source
        D3D12_TEXTURE_COPY_LOCATION src = {};
        src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        src.pResource = ptrSrc->Ptr();
        src.PlacedFootprint.Offset = bufferOffset;
        src.PlacedFootprint.Footprint.Width = m_width;
        src.PlacedFootprint.Footprint.Height = m_height;
        src.PlacedFootprint.Footprint.Depth = m_depth;
        src.PlacedFootprint.Footprint.Format = m_format;
        src.PlacedFootprint.Footprint.RowPitch = m_width * m_bytesPerPixel;

        // Destination
        D3D12_TEXTURE_COPY_LOCATION dest = {};
        dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dest.pResource = Ptr();
        dest.SubresourceIndex = 0;

        // Src box
        D3D12_BOX srcBox;
        srcBox.left = srcBox.top = srcBox.front = 0;
        srcBox.right = m_width;
        srcBox.bottom = m_height;
        srcBox.back = m_depth;

        // Execute copy
        ptrCmdList->Ptr()->CopyTextureRegion(&dest, 0, 0, 0, &src, &srcBox);
    }

    return canCopy;
}

bool MGX::Core::GType::Texture::CopyToBuffer(GPU::CommandList* ptrCmdList, GPU::Resource* ptrDest, UINT bufferOffset, bool allowBarries)
{
    // Vertify size
    bool canCopy = CheckSetResourceBarriers(ptrCmdList, this, ptrDest, allowBarries);

    // Copy
    if (canCopy)
    {
        // Source
        D3D12_TEXTURE_COPY_LOCATION src = {};
        src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        src.pResource = ptrDest->Ptr();
        src.SubresourceIndex = 0;

        // Destination
        D3D12_TEXTURE_COPY_LOCATION dest = {};
        dest.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        dest.pResource = Ptr();
        dest.PlacedFootprint.Offset = bufferOffset;
        dest.PlacedFootprint.Footprint.Width = m_width;
        dest.PlacedFootprint.Footprint.Height = m_height;
        dest.PlacedFootprint.Footprint.Depth = m_depth;
        dest.PlacedFootprint.Footprint.Format = m_format;
        dest.PlacedFootprint.Footprint.RowPitch = m_width * m_bytesPerPixel;

        // Src box
        D3D12_BOX srcBox;
        srcBox.left = srcBox.top = srcBox.front = 0;
        srcBox.right = m_width;
        srcBox.bottom = m_height;
        srcBox.back = m_depth;

        // Execute copy
        ptrCmdList->Ptr()->CopyTextureRegion(&dest, 0, 0, 0, &src, &srcBox);
    }

    return canCopy;
}

void MGX::Core::GType::Texture::CreateSRV(ID3D12Device* ptrDevice, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT mostDetailedMip, FLOAT resourceMinLODClamp)
{
    // Basic description
    D3D12_SHADER_RESOURCE_VIEW_DESC vd = {};
    vd.Format = m_format;
    vd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    // Dependent desc
    switch (m_dimension)
    {
        case TextureDimension::Texture1D:
            vd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
            vd.Texture1D.MipLevels = m_mipCount;
            vd.Texture1D.MostDetailedMip = mostDetailedMip;
            vd.Texture1D.ResourceMinLODClamp = resourceMinLODClamp;
            break;
        case TextureDimension::Texture2D:
            vd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            vd.Texture2D.MipLevels = m_mipCount;
            vd.Texture2D.PlaneSlice = 0;
            vd.Texture2D.MostDetailedMip = mostDetailedMip;
            vd.Texture2D.ResourceMinLODClamp = resourceMinLODClamp;
            break;
        case TextureDimension::Texture3D:
            vd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
            vd.Texture3D.MipLevels = m_mipCount;
            vd.Texture3D.MostDetailedMip = mostDetailedMip;
            vd.Texture3D.ResourceMinLODClamp = resourceMinLODClamp;
            break;
        default:
            throw std::exception("Canot create SRV for invalid texture!");
    }

    // Create SRV
    ptrDevice->CreateShaderResourceView(m_ptrBase, &vd, handle);
}

bool MGX::Core::GType::Texture::CheckSetResourceBarriers(GPU::CommandList* ptrCmdList, GPU::Resource* ptrSrc, GPU::Resource* ptrDest, bool allowBarriers)
{
    // Resource barriers
    if (allowBarriers)
    {
        if (ptrSrc->SetResourceState(D3D12_RESOURCE_STATE_COPY_SOURCE, ptrCmdList->ResourceBarrierPeek()))
            ptrCmdList->ResourceBarrierPush();
        if (ptrDest->SetResourceState(D3D12_RESOURCE_STATE_COPY_DEST, ptrCmdList->ResourceBarrierPeek()))
            ptrCmdList->ResourceBarrierPush();
        ptrCmdList->ResourceBarrierFlush();

        // Ok
        return true;
    }
    else
    {
        // Check current state
        return ptrSrc->GetResourceState() == D3D12_RESOURCE_STATE_COPY_SOURCE && ptrDest->GetResourceState() == D3D12_RESOURCE_STATE_COPY_DEST;
    }
}
