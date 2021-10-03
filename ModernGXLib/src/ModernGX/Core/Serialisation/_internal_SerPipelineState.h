#pragma once

#include <tinyxml2/tinyxml2.h>

#include <sstream>

#include <ModernGX.h>
#include <ModernGX/Util/Memory.h>
#include <ModernGX/Util/StrHashTable.h>
#include <ModernGX/Util/TinyxmlHelpers.h>
#include <ModernGX/Core/Allocator/StackMemoryAllocator.h>

namespace MGX::Core::Serialisation
{
    class PipelineState_helpers
    {
        public:
            // Do not construct
            PipelineState_helpers() = delete;
            PipelineState_helpers(const PipelineState_helpers&) = delete;

            // Hash genneration trigger
            static void GennerateHashValues() noexcept;

            // Deserialize GFX State from file
            static bool DSerGfxPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC* ptrStateDesc, const char** ppPsoName, tinyxml2::XMLElement* ptrRootElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

        private:
            // String hash table
            static ::MGX::Serialisation::StringHashTable<D3D12_BLEND> s_D3D12_BLEND_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP> s_D3D12_BLEND_OP_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP> s_D3D12_LOGIC_OP_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_FILL_MODE> s_D3D12_FILL_MODE_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_CULL_MODE> s_D3D12_CULL_MODE_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_CONSERVATIVE_RASTERIZATION_MODE> s_D3D12_CONSERVATIVE_RASTERIZATION_MODE_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_DEPTH_WRITE_MASK> s_D3D12_DEPTH_WRITE_MASK_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC> s_D3D12_COMPARISON_FUNC_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP> s_D3D12_STENCIL_OP_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE> s_D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE> s_D3D12_PRIMITIVE_TOPOLOGY_TYPE_table[];
            static ::MGX::Serialisation::StringHashTable<DXGI_FORMAT> s_DXGI_FORMAT_table[];
            static ::MGX::Serialisation::StringHashTable<D3D12_INPUT_CLASSIFICATION> s_D3D12_INPUT_CLASSIFICATION_table[];

        private:
            // Load shader
            static bool DSerLoadShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC* ptrStateDesc, const char* shaderType, const char* shaderPath, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;
            
            // Load D3D12_STREAM_OUTPUT_DESC
            static bool DSerLoadStreamOutput(D3D12_STREAM_OUTPUT_DESC* ptrSODesc, tinyxml2::XMLElement* ptrSOElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load D3D12_SO_DECLARATION_ENTRY
            static bool DSerLoadSOEntry(D3D12_SO_DECLARATION_ENTRY* ptrSOEntrie, tinyxml2::XMLElement* ptrSOEntrieElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load D3D12_BLEND_DESC
            static bool DSerLoadBlendState(D3D12_BLEND_DESC* ptrBlendDesc, UINT32* ptrSampleMask, tinyxml2::XMLElement* ptrBlendDescElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;
    
            // Load D3D12_RENDER_TARGET_BLEND_DESC
            static bool DSerLoadBlendStateRT(D3D12_RENDER_TARGET_BLEND_DESC* ptrBlendRtDesc, tinyxml2::XMLElement* ptrBlendRtDescElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load D3D12_RASTERIZER_DESC 
            static bool DSerLoadRasterizerState(D3D12_RASTERIZER_DESC* ptrRasterizerState, tinyxml2::XMLElement* ptrRasterizerStateElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load D3D12_DEPTH_STENCIL_DESC 
            static bool DSerDepthStencilState(D3D12_DEPTH_STENCIL_DESC* ptrDepthStencilState, tinyxml2::XMLElement* ptrDepthStencilStateElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load D3D12_DEPTH_STENCILOP_DESC
            static bool DSerStencilOp(D3D12_DEPTH_STENCILOP_DESC* ptrStencilOp, tinyxml2::XMLElement* ptrStencilOpElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load DXGI_SAMPLE_DESC 
            static bool DSerSampleDesc(DXGI_SAMPLE_DESC* ptrSampleDesc, tinyxml2::XMLElement* ptrSampleDescElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load D3D12_INPUT_LAYOUT_DESC
            static bool DSerInputLayouts(D3D12_INPUT_LAYOUT_DESC* ptrInputLayout, tinyxml2::XMLElement* ptrInputLayoutsElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load D3D12_INPUT_ELEMENT_DESC
            static bool DSerInputLayoutElement(D3D12_INPUT_ELEMENT_DESC* ptrInputLayoutElement, tinyxml2::XMLElement* ptrInputLayoutElementElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;

            // Load RenderTargets
            static bool DSerRenderTargets(DXGI_FORMAT* ptrDSVFormat, DXGI_FORMAT* ptrRTVFormats, UINT* ptrNumRTV, tinyxml2::XMLElement* ptrRenderTargetsElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept;
    };
}
