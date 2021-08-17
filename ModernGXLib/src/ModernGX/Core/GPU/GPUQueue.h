#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>

#include <exception>

namespace MGX {
    namespace Core {
        namespace GPU {
            // Command queue
            class CommandQueue : public Foundation::COMGetable<ID3D12CommandQueue> {
                public:
                    // Construct
                    CommandQueue() = default;
                    CommandQueue(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_QUEUE_PRIORITY prio = D3D12_COMMAND_QUEUE_PRIORITY_HIGH);
                    CommandQueue(const CommandQueue&) = delete;

                    // Destruct
                    ~CommandQueue();

                    // Operators
                    CommandQueue& operator=(CommandQueue&& other) noexcept;

                    // Execute function
                    UINT64 Execute(ID3D12CommandList* ptrCommandList) noexcept;
                    // Wait functions
                    void Wait(UINT64 value = UINT64_MAX) noexcept;
                    // Flush function
                    void Flush(unsigned int count) noexcept;
                    
                private:
                    // Fence for command queue
                    ComPointer<ID3D12Fence> m_ptrFence;
                    UINT64 m_fenceValue = 0;

                    // Wait event
                    HANDLE m_waitEvent = INVALID_HANDLE_VALUE;

                    // Type of command queue
                    D3D12_COMMAND_LIST_TYPE m_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            };
        }
    }
}
