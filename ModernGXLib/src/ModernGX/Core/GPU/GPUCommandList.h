#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>

#include <exception>

namespace MGX::Core::GPU {
    // Command queue
    class CommandList : public Foundation::COMGetable<ID3D12GraphicsCommandList> 
    {
        public:
            // Construct
            CommandList() = default;
            CommandList(const CommandList&) = delete;
            CommandList(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

            // Copy
            CommandList& operator=(const CommandList&) = delete;

            // Assign move
            CommandList& operator=(CommandList&& other) noexcept;

            // Close and reset command list
            void Close() noexcept;
            void Reset() noexcept;
            bool IsOpen() noexcept;

            // Peek next barrier
            inline D3D12_RESOURCE_BARRIER* ResourceBarrierPeak() noexcept
            {
                return &m_resBarriers[m_barrierUsage];
            }
            // Incrment barrieren usage
            inline void ResourceBarrierPush() noexcept
            {
                m_barrierUsage++;
            }
            // Flush ressource barriere execution
            void ResourceBarrierFlush() noexcept;

            // Name command list
            HRESULT name(LPCWSTR name) override;

        private:
            // Command allocator
            ComPointer<ID3D12CommandAllocator> m_ptrAllocator;

            // Resource Barrieres
            D3D12_RESOURCE_BARRIER m_resBarriers[256];
            UINT m_barrierUsage = 0;

            // Indicates command list state
            bool m_bOpen = true;
    };
}
