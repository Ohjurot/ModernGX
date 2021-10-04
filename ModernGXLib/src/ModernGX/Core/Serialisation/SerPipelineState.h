#pragma once

#include <ModernGX.h>
#include <ModernGX/Util/File.h>
#include <ModernGX/Core/Allocator/IAllocator.h>
#include <ModernGX/Core/Allocator/StackMemoryAllocator.h>

namespace MGX::Core::Serialisation
{
    class PipelineState
    {
        public:
            // Dont OOP me
            PipelineState() = delete;
            PipelineState(const PipelineState&) = delete;
            PipelineState& operator=(const PipelineState&) = delete;

            // Init function
            static void Init() noexcept;

            // Load PSO from disk
            static bool LoadPipelineStateFromDisk(ID3D12RootSignature** ppRootSignature, ID3D12PipelineState** ppPipelineState, const wchar_t* xmlSubFilePath, IAllocator* ptrAllocator = StdAllocator::Instance()) noexcept;
    };
}
