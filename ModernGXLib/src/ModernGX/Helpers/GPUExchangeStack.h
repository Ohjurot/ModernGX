#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/GPU/GPUQueue.h>
#include <ModernGX/Core/GPU/GPUCommandList.h>
#include <ModernGX/Core/GPUTypes/GTypeBuffer.h>

namespace MGX::Helpers
{
    // Helper class for exchanging GPU <--> CPU data
    class GPUExchangeBuffer
    {
        public:


        private:
            // Command list for copying data
            Core::GPU::CommandList m_copyCommandList;
            
            // Wait state of command list
            Core::GPU::CommandQueue* m_ptrActiveQueue = nullptr;
            UINT64 m_activeQueueWaitValue = UINT64_MAX;

            // Exchange buffer (CPU read or write) and type
            Core::GType::Buffer m_sharedBuffer;
            bool m_isDownloadBuffer = false;

    };
}
