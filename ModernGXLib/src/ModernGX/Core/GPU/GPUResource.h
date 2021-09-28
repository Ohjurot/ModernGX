#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GPU/GPUHeap.h>

#include <exception>

namespace MGX::Core::GPU
{
    // Common GPU Resource
    class Resource : public Foundation::COMGetable<ID3D12Resource>
    {
        public:
            // Construct
            Resource() = default;
            Resource(const Resource&) = delete;
            Resource(ID3D12Resource* ptrResource, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON) noexcept;
            Resource(ID3D12Device* ptrDevice, HeapUsage usage, const D3D12_RESOURCE_DESC* ptrDesc, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON, 
                const D3D12_CLEAR_VALUE* ptrClearValue = nullptr, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);
            Resource(ID3D12Device* ptrDevice, const D3D12_RESOURCE_DESC* ptrDesc, HeapAllocationCookie allocationCookie, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON,
                const D3D12_CLEAR_VALUE* ptrClearValue = nullptr);

            // Copy
            Resource& operator=(const Resource&) = delete;

            // Move
            Resource& operator=(Resource&& other) noexcept;

            // Retrive state
            inline D3D12_RESOURCE_STATES GetResourceState() const noexcept
            {
                return m_currentState;
            }

            // Set state (via barrier)
            bool SetResourceState(D3D12_RESOURCE_STATES newState, D3D12_RESOURCE_BARRIER* ptrBarrier) noexcept;

        protected:
            // Set resource function
            inline void RxSet(Resource& other)
            {
                this->operator=(std::move(other));
            }

        private:
            // Resource state
            D3D12_RESOURCE_STATES m_currentState = D3D12_RESOURCE_STATE_COMMON;
    };
}
