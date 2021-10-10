#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/Serialisation/SerPipelineState.h>

#include <exception>

#define MGX_CORE_GPU_PIPELINESTATE_FROM_FILE 1
#define MGX_CORE_GPU_PIPELINESTATE_FROM_STRING 2

namespace MGX::Core::GPU
{
    // Type of PSO
    enum class PipelineStateType
    {
        // Invalid state
        Invalid,
        // Graffics pipeline state
        Graffics,
        // Compute pipeline state
        Compute,
    };

    class PipelineState
    {
        public:
            // Construct
            PipelineState() = default;
            PipelineState(const PipelineState&) = default; // This can actualy copy
            PipelineState(ID3D12Device* ptrDevice, const wchar_t* configFileName, IAllocator* ptrAllocator = StdAllocator::Instance()); // From .xml file
            PipelineState(ID3D12Device* ptrDevice, ID3D12RootSignature* ptrSignature, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* gfxDescription); // For GFX
            PipelineState(ID3D12Device* ptrDevice, ID3D12RootSignature* ptrSignature, const D3D12_COMPUTE_PIPELINE_STATE_DESC* computeDescription); // For Compute
            // PipelineState(ID3D12Device* ptrDevice, const wchar_t* computeShaderFile, IAllocator* ptrAllocator = StdAllocator::Instance()); // For Compute (File)

            // Get type
            inline PipelineStateType GetType() const noexcept
            {
                return m_type;
            }

            // Bind to command list
            void Bind(ID3D12GraphicsCommandList* ptrCommandList);

        private:
            // Construct class from blob
            static void __constructFromBlob(ID3D12Device* ptrDevice, PipelineState* ptrInstance, void* buffer, size_t len);

        private:
            // State
            PipelineStateType m_type = PipelineStateType::Invalid;

            // DirectX Objects
            ComPointer<ID3D12PipelineState> m_pipelineState;
            ComPointer<ID3D12RootSignature> m_rootSignature;
    };
}
