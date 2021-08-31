#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>

#include <exception>

namespace MGX {
    namespace Core {
        namespace GPU {
            // Command queue
            class CommandList : public Foundation::COMGetable<ID3D12GraphicsCommandList> {
                public:
                    // Construct
                    CommandList() = default;
                    CommandList(const CommandList&) = delete;
                    CommandList(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

                    // Assign move
                    CommandList& operator=(CommandList&& other) noexcept;

                    // Close and reset command list
                    void Close() noexcept;
                    void Reset() noexcept;
                    bool IsOpen() noexcept;

                    // Name command list
                    HRESULT name(LPCWSTR name) override {
                        HRESULT hr;
                        if (FAILED(hr = m_ptrBase.name(name))) return hr;
                        if (FAILED(hr = m_ptrAllocator.name(name))) return hr;
                        return hr;
                    }

                private:
                    // Command allocator
                    ComPointer<ID3D12CommandAllocator> m_ptrAllocator;

                    // Indicates command list state
                    bool m_bOpen = true;
            };
        }
    }
}
