#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GPU/GPUPipelineState.h>

#include <exception>

namespace MGX::Core::GPU
{
    // Root configuration entry
    enum class RootConfigurationEntry_t
    {
        // Invalid entry
        Invalid = 0,
        // Root Constant
        RootConstant,
        // CBV
        ConstantBufferView,
        // SRV
        ShaderResourceView,
        // UAV
        UnorderedAccessView,
        // Descriptor Table
        DescriptorTable,
    };

    // Defines a root element
    struct RootConfigurationEntry
    {
        // Entity type
        RootConfigurationEntry_t type;
        
        // Data container
        union 
        {
            // For type == RootConstant
            struct
            {
                // Count of 32-Bit values
                UINT valueCount = 0;
                // Pointer to cpu source data
                void* ptrData = nullptr;
            }
            RootConstant;

            // For type == ConstantBufferView
            struct
            {
                // Address of CBV data
                D3D12_GPU_VIRTUAL_ADDRESS dataAddress = 0;
            }
            ConstantBufferView;

            // For type == ShaderResourceView
            struct
            {
                // Address of SRV data
                D3D12_GPU_VIRTUAL_ADDRESS dataAddress = 0;
            }
            ShaderResourceView;

            // For type == UnorderedAccessView
            struct
            {
                // Address of the UAV data
                D3D12_GPU_VIRTUAL_ADDRESS dataAddress = 0;
            }
            UnorderedAccessView;

            // For type == DescriptorTable
            struct
            {
                // Address of the base descriptor
                D3D12_GPU_DESCRIPTOR_HANDLE baseDescriptor = {0};
            }
            DescriptorTable;
        };
    };

    // Root signature configuration
    class RootConfiguration
    {
        public:
            // Construct
            inline RootConfiguration(PipelineStateType type) : m_type(type) {};
            RootConfiguration(const RootConfiguration&) = default;
            
            template<typename... T, typename = std::enable_if_t<std::is_same_v<T..., RootConfigurationEntry>>>
            RootConfiguration(PipelineStateType type, T... args) :
                m_type(type)
            {
                // Unpack
                if (!PushBack(args...))
                {
                    throw std::exception("To many variadic elements in RootConfiguration constructor");
                }
            }

            // Push back new entry
            bool PushBack(RootConfigurationEntry other) noexcept;

            // Get element to edit
            RootConfigurationEntry& operator[](unsigned int index);

            // Current size
            inline unsigned int Size() const noexcept
            {
                return m_usage;
            }

            // Total max capacity
            static constexpr unsigned int Capacity()
            {
                return 32;
            }

            // Bind to CommandList
            bool Bind(ID3D12GraphicsCommandList* ptrCmdList);

        private:
            // Binding helper
            void __bindHelper(ID3D12GraphicsCommandList* ptrCmdList, unsigned int index, const RootConfigurationEntry* ptrEntry);

        private:
            // Type of list
            PipelineStateType m_type = PipelineStateType::Invalid;

            // Entry buffer
            RootConfigurationEntry m_entrys[32] = {};

            // Entry usage
            unsigned int m_usage = 0;
    };
}
