#pragma once

#include <ModernGX.h>
#include <ModernGX/Util/DXGIFormat.h>
#include <ModernGX/Core/GPU/GPUHeap.h>
#include <ModernGX/Core/GPU/GPUResource.h>
#include <ModernGX/Core/GPU/GPUCommandList.h>

namespace MGX::Core::GType
{
    // Dimension of texture
    enum class TextureDimension
    {
        Invalid = 0,
        Texture1D,
        Texture2D,
        Texture3D,
    };

    // texture
    class Texture : public GPU::Resource
    {
        public:
            // Construct
            Texture() = default;
            Texture(const Texture&) = delete;
            Texture(ID3D12Device* ptrDevice, GPU::HeapUsage usage, DXGI_FORMAT textureFormat, UINT width, UINT height, UINT16 depth, INT mipLevels = 1, UINT sampleCount = 1, UINT sampleQuality = 0,
                D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE); // Commited
            Texture(ID3D12Device* ptrDevice, GPU::HeapAllocationCookie allocationCookie, DXGI_FORMAT textureFormat, UINT width, UINT height, UINT16 depth, INT mipLevels = 1, UINT sampleCount = 1, UINT sampleQuality = 0,
                D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN); // Placed
            Texture(ID3D12Device* ptrDevice, DXGI_FORMAT textureFormat, UINT width, UINT height, UINT16 depth, INT mipLevels = 1, UINT sampleCount = 1, UINT sampleQuality = 0,
                D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN); // Reserved

            // Delete copy
            Texture& operator=(const Texture&) = delete;

            // Only move
            Texture& operator=(Texture&& other) noexcept;

            // Full Copy function
            bool CopyFrom(GPU::CommandList* ptrCmdList, Texture* ptrSrc, bool allowBarriers = true); 
            bool CopyTo(GPU::CommandList* ptrCmdList, Texture* ptrDest, bool allowBarriers = true);
            
            // Copy from buffer (Upload/Download)
            bool CopyFromBuffer(GPU::CommandList* ptrCmdList, GPU::Resource* ptrSrc, UINT64 bufferOffset = 0, bool allowBarries = true);
            bool CopyToBuffer(GPU::CommandList* ptrCmdList, GPU::Resource* ptrDest, UINT64 bufferOffset = 0, bool allowBarries = true);

            // Create SRV for texture
            void CreateSRV(ID3D12Device* ptrDevice, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT mostDetailedMip = 0, FLOAT resourceMinLODClamp = 0.0f);

            // Description gennerator
            static constexpr D3D12_RESOURCE_DESC CreateResourceDesc(DXGI_FORMAT format, UINT width, UINT height, UINT16 depth,
                UINT mipLevels = 1, UINT sampleCount = 1, UINT sampleQuality = 0, 
                D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
            {
                D3D12_RESOURCE_DESC rd;

                // Dimension
                if (depth > 1)
                    rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
                else if (height > 1)
                    rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                else
                    rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;

                // Default Description
                rd.Alignment = sampleCount == 1 ? D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT: D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT;
                rd.Width = width;
                rd.Height = height ? height : 1;
                rd.DepthOrArraySize = depth ? depth : 1;
                rd.MipLevels = mipLevels;
                rd.Format = format;
                rd.SampleDesc.Count = sampleCount;
                rd.SampleDesc.Quality = sampleQuality;
                rd.Layout = layout;
                rd.Flags = flags;
                return rd;
            }

            // Getter helpers
            inline UINT GetWidth() const noexcept
            {
                return m_width;
            }
            inline UINT GetHeight() const noexcept
            {
                return m_height;
            }
            inline UINT GetDepth() const noexcept
            {
                return m_depth;
            }
            inline UINT GetMipCount() const noexcept
            {
                return m_mipCount;
            }
            inline DXGI_FORMAT GetFormat() const noexcept
            {
                return m_format;
            }
            inline TextureDimension GetDimension() const noexcept
            {
                return m_dimension;
            }

        private:
            // Internal resource barrier helper
            static bool CheckSetResourceBarriers(GPU::CommandList* ptrCmdList, GPU::Resource* ptrSrc, GPU::Resource* ptrDest, bool allowBarriers);

        private:
            // Texture parameters
            UINT m_width = 0, m_height = 0, m_depth = 0;
            TextureDimension m_dimension = TextureDimension::Invalid;
            UINT m_mipCount = 1;
            DXGI_FORMAT m_format = DXGI_FORMAT_UNKNOWN;
            UINT m_bytesPerPixel = 0;
    };
}
