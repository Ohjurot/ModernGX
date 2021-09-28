#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/Allocator/ListMemoryAccelerator.h>

namespace MGX::Core::GPU
{
    // Predecl
    class DescriptorHeap;

    // Heap usage (RTV/SRV/...
    enum class DescriptorHeapUsage
    {
        // Not used
        Null = 10,

        // Used for RenderTargetViews
        RenderTargetView = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,

        // Used for DepthStencilViews
        DepthStencilView = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,

        // Used for samplers
        SamplerDesc = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,

        // Used for shader resources
        ShaderResource = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
    };

    // Descriptor pair
    struct DescriptorPair
    {
        D3D12_GPU_DESCRIPTOR_HANDLE gpu = { 0 };
        D3D12_CPU_DESCRIPTOR_HANDLE cpu = { 0 };
    };

    // Range of descriptors
    class DescriptorRange
    {
        public:
            // Construct
            DescriptorRange() = default;
            DescriptorRange(const DescriptorRange&) = default;
            DescriptorRange(D3D12_GPU_DESCRIPTOR_HANDLE baseGpu, D3D12_CPU_DESCRIPTOR_HANDLE baseCpu, UINT increment, UINT32 range);

            // Copy
            DescriptorRange& operator=(const DescriptorRange&) = default;

            // Get size of range
            inline UINT32 GetSize() const noexcept
            {
                return m_rangeSize;
            }

            // Get descriptor pair
            DescriptorPair Get(UINT32 index);
            
            // Get operator
            inline DescriptorPair operator[](UINT32 index)
            {
                return Get(index);
            }

            // Valid check
            inline bool IsValid() const noexcept
            {
                return m_rangeSize != 0;
            }

            // Boolean conversion (is valid)
            inline operator bool() const noexcept
            {
                return IsValid();
            }
        
        private:
            // Allow descriptor
            friend class DescriptorHeap;

            // Base addresses
            D3D12_GPU_DESCRIPTOR_HANDLE m_baseAddressGpu = { 0 };
            D3D12_CPU_DESCRIPTOR_HANDLE m_baseAddressCpu = { 0 };

            // Range size
            UINT32 m_rangeSize = 0;

            // Increment step
            UINT m_incrementStep = 0;
    };

    // Gpu Descriptor heap
    class DescriptorHeap : public Foundation::COMGetable<ID3D12DescriptorHeap>
    {
        public:
            // Construct
            DescriptorHeap() = default;
            DescriptorHeap(const DescriptorHeap&) = delete;
            DescriptorHeap(ID3D12Device* ptrDevice, DescriptorHeapUsage usage, UINT32 descriptorCount, IAllocator* ptrCpuAllocator = StdAllocator::Instance());

            // Copy
            DescriptorHeap& operator=(const DescriptorHeap&) = delete;

            // Move
            DescriptorHeap& operator=(DescriptorHeap&& other) noexcept;

            // Allocate desc heap range
            DescriptorRange Allocate(UINT32 count = 1) noexcept;

            // Free range
            void Free(const DescriptorRange* range) noexcept;

            // Get usage of this hap
            inline DescriptorHeapUsage GetUsage() const noexcept
            {
                return m_usage;
            }

        private:
            // Acceleration structure
            Allocator::ListMemoryAccelerator m_ac;

            // Type
            DescriptorHeapUsage m_usage = DescriptorHeapUsage::Null;

            // Descritor base addresses
            D3D12_GPU_DESCRIPTOR_HANDLE m_baseGpu = { 0 };
            D3D12_CPU_DESCRIPTOR_HANDLE m_baseCpu = { 0 };

            // Descriptor increment
            UINT m_descriptorIncrement = 0;
    };
}
