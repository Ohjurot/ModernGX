#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/GPU/GPUQueue.h>
#include <ModernGX/Core/GPU/GPUCommandList.h>
#include <ModernGX/Core/GPUTypes/GTypeBuffer.h>
#include <ModernGX/Core/GPUTypes/GTypeTexture.h>
#include <ModernGX/Coding/TextureCoding.h>

namespace MGX::Helpers
{
    // Helper class for exchanging GPU <--> CPU data
    class GPUUploadStack
    {
        public:
            // Construct
            GPUUploadStack() = default;
            GPUUploadStack(const GPUUploadStack&) = delete;
            GPUUploadStack(ID3D12Device* ptrDevice, UINT64 size); // Commited
            GPUUploadStack(ID3D12Device* ptrDevice, Core::GPU::HeapAllocationCookie heapAllocation, UINT64 size); // Placed

            // Assign
            GPUUploadStack& operator=(const GPUUploadStack&) = delete;
            GPUUploadStack& operator=(GPUUploadStack&& other) noexcept;

            // Reset internal head pointer and set new commands list (or keep)
            bool Reset(Core::GPU::CommandList* ptrCmdList = nullptr);
            void Close();

            // Copy buffer
            bool CopyBuffer(Core::GType::Buffer* ptrDest, const void* srcData, UINT64 size, UINT64 destOffset = 0);
            // Copy texture
            bool CopyTexture(Core::GType::Texture* ptrDstTexture, Coding::TextureFile* ptrSrcTexture);

        private:
            // Command list for copying data
            Core::GPU::CommandList* m_ptrCurrentCommandList = nullptr;

            // Exchange buffer (CPU read or write) and type
            Core::GType::Buffer m_sharedBuffer;
            Core::GType::MappedBuffer m_mappedBuffer;

            // Head pointer
            UINT64 m_head = 0;
            UINT64 m_size = 0;
    };
}
