#include "ModernGX.h"
#include "_internal_SerPipelineState.h"

::MGX::Serialisation::StringHashTable<D3D12_BLEND> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_BLEND_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_ZERO", D3D12_BLEND_ZERO),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_ONE", D3D12_BLEND_ONE),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_SRC_COLOR", D3D12_BLEND_SRC_COLOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_INV_SRC_COLOR", D3D12_BLEND_INV_SRC_COLOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_SRC_ALPHA", D3D12_BLEND_SRC_ALPHA),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_INV_SRC_ALPHA", D3D12_BLEND_INV_SRC_ALPHA),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_DEST_ALPHA", D3D12_BLEND_DEST_ALPHA),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_INV_DEST_ALPHA", D3D12_BLEND_INV_DEST_ALPHA),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_DEST_COLOR", D3D12_BLEND_DEST_COLOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_INV_DEST_COLOR", D3D12_BLEND_INV_DEST_COLOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_SRC_ALPHA_SAT", D3D12_BLEND_SRC_ALPHA_SAT),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_BLEND_FACTOR", D3D12_BLEND_BLEND_FACTOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_INV_BLEND_FACTOR", D3D12_BLEND_INV_BLEND_FACTOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_SRC1_COLOR", D3D12_BLEND_SRC1_COLOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_INV_SRC1_COLOR", D3D12_BLEND_INV_SRC1_COLOR),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_SRC1_ALPHA", D3D12_BLEND_SRC1_ALPHA),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Entrie("D3D12_BLEND_INV_SRC1_ALPHA", D3D12_BLEND_INV_SRC1_ALPHA),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_BLEND_OP_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP>::Entrie("D3D12_BLEND_OP_ADD", D3D12_BLEND_OP_ADD),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP>::Entrie("D3D12_BLEND_OP_SUBTRACT", D3D12_BLEND_OP_SUBTRACT),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP>::Entrie("D3D12_BLEND_OP_REV_SUBTRACT", D3D12_BLEND_OP_REV_SUBTRACT),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP>::Entrie("D3D12_BLEND_OP_MIN", D3D12_BLEND_OP_MIN),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP>::Entrie("D3D12_BLEND_OP_MAX", D3D12_BLEND_OP_MAX),
    ::MGX::Serialisation::StringHashTable<D3D12_BLEND_OP>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_LOGIC_OP_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_CLEAR", D3D12_LOGIC_OP_CLEAR),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_SET", D3D12_LOGIC_OP_SET),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_COPY", D3D12_LOGIC_OP_COPY),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_COPY_INVERTED", D3D12_LOGIC_OP_COPY_INVERTED),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_NOOP", D3D12_LOGIC_OP_NOOP),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_INVERT", D3D12_LOGIC_OP_INVERT),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_AND", D3D12_LOGIC_OP_AND),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_NAND", D3D12_LOGIC_OP_NAND),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_OR", D3D12_LOGIC_OP_OR),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_NOR", D3D12_LOGIC_OP_NOR),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_XOR", D3D12_LOGIC_OP_XOR),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_EQUIV", D3D12_LOGIC_OP_EQUIV),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_AND_REVERSE", D3D12_LOGIC_OP_AND_REVERSE),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_AND_INVERTED", D3D12_LOGIC_OP_AND_INVERTED),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_OR_REVERSE", D3D12_LOGIC_OP_OR_REVERSE),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Entrie("D3D12_LOGIC_OP_OR_INVERTED", D3D12_LOGIC_OP_OR_INVERTED),
    ::MGX::Serialisation::StringHashTable<D3D12_LOGIC_OP>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_FILL_MODE> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_FILL_MODE_table[] = 
{
    ::MGX::Serialisation::StringHashTable<D3D12_FILL_MODE>::Entrie("D3D12_FILL_MODE_WIREFRAME", D3D12_FILL_MODE_WIREFRAME),
    ::MGX::Serialisation::StringHashTable<D3D12_FILL_MODE>::Entrie("D3D12_FILL_MODE_SOLID", D3D12_FILL_MODE_SOLID),
    ::MGX::Serialisation::StringHashTable<D3D12_FILL_MODE>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_CULL_MODE> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_CULL_MODE_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_CULL_MODE>::Entrie("D3D12_CULL_MODE_NONE", D3D12_CULL_MODE_NONE),
    ::MGX::Serialisation::StringHashTable<D3D12_CULL_MODE>::Entrie("D3D12_CULL_MODE_FRONT", D3D12_CULL_MODE_FRONT),
    ::MGX::Serialisation::StringHashTable<D3D12_CULL_MODE>::Entrie("D3D12_CULL_MODE_BACK", D3D12_CULL_MODE_BACK),
    ::MGX::Serialisation::StringHashTable<D3D12_CULL_MODE>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_CONSERVATIVE_RASTERIZATION_MODE> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_CONSERVATIVE_RASTERIZATION_MODE_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_CONSERVATIVE_RASTERIZATION_MODE>::Entrie("D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF", D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF),
    ::MGX::Serialisation::StringHashTable<D3D12_CONSERVATIVE_RASTERIZATION_MODE>::Entrie("D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON", D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON),
    ::MGX::Serialisation::StringHashTable<D3D12_CONSERVATIVE_RASTERIZATION_MODE>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_DEPTH_WRITE_MASK> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_DEPTH_WRITE_MASK_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_DEPTH_WRITE_MASK>::Entrie("D3D12_DEPTH_WRITE_MASK_ZERO", D3D12_DEPTH_WRITE_MASK_ZERO),
    ::MGX::Serialisation::StringHashTable<D3D12_DEPTH_WRITE_MASK>::Entrie("D3D12_DEPTH_WRITE_MASK_ALL", D3D12_DEPTH_WRITE_MASK_ALL),
    ::MGX::Serialisation::StringHashTable<D3D12_DEPTH_WRITE_MASK>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_COMPARISON_FUNC_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_NEVER", D3D12_COMPARISON_FUNC_NEVER),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_LESS", D3D12_COMPARISON_FUNC_LESS),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_EQUAL", D3D12_COMPARISON_FUNC_EQUAL),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_LESS_EQUAL", D3D12_COMPARISON_FUNC_LESS_EQUAL),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_GREATER", D3D12_COMPARISON_FUNC_GREATER),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_NOT_EQUAL", D3D12_COMPARISON_FUNC_NOT_EQUAL),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_GREATER_EQUAL", D3D12_COMPARISON_FUNC_GREATER_EQUAL),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Entrie("D3D12_COMPARISON_FUNC_ALWAYS", D3D12_COMPARISON_FUNC_ALWAYS),
    ::MGX::Serialisation::StringHashTable<D3D12_COMPARISON_FUNC>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_STENCIL_OP_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_KEEP", D3D12_STENCIL_OP_KEEP),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_ZERO", D3D12_STENCIL_OP_ZERO),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_REPLACE", D3D12_STENCIL_OP_REPLACE),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_INCR_SAT", D3D12_STENCIL_OP_INCR_SAT),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_DECR_SAT", D3D12_STENCIL_OP_DECR_SAT),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_INVERT", D3D12_STENCIL_OP_INVERT),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_INCR", D3D12_STENCIL_OP_INCR),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Entrie("D3D12_STENCIL_OP_DECR", D3D12_STENCIL_OP_DECR),
    ::MGX::Serialisation::StringHashTable<D3D12_STENCIL_OP>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>::Entrie("D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED", D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED),
    ::MGX::Serialisation::StringHashTable<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>::Entrie("D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF", D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF),
    ::MGX::Serialisation::StringHashTable<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>::Entrie("D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF", D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF),
    ::MGX::Serialisation::StringHashTable<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_PRIMITIVE_TOPOLOGY_TYPE_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE>::Entrie("D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED", D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED),
    ::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE>::Entrie("D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT", D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT),
    ::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE>::Entrie("D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE", D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE),
    ::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE>::Entrie("D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE", D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE),
    ::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE>::Entrie("D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH", D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH),
    ::MGX::Serialisation::StringHashTable<D3D12_PRIMITIVE_TOPOLOGY_TYPE>::Terminator()
};

::MGX::Serialisation::StringHashTable<DXGI_FORMAT> MGX::Core::Serialisation::PipelineState_helpers::s_DXGI_FORMAT_table[] =
{
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_UNKNOWN", DXGI_FORMAT_UNKNOWN),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32A32_TYPELESS", DXGI_FORMAT_R32G32B32A32_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32A32_FLOAT", DXGI_FORMAT_R32G32B32A32_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32A32_UINT", DXGI_FORMAT_R32G32B32A32_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32A32_SINT", DXGI_FORMAT_R32G32B32A32_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32_TYPELESS", DXGI_FORMAT_R32G32B32_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32_FLOAT", DXGI_FORMAT_R32G32B32_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32_UINT", DXGI_FORMAT_R32G32B32_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32B32_SINT", DXGI_FORMAT_R32G32B32_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16B16A16_TYPELESS", DXGI_FORMAT_R16G16B16A16_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16B16A16_FLOAT", DXGI_FORMAT_R16G16B16A16_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16B16A16_UNORM", DXGI_FORMAT_R16G16B16A16_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16B16A16_UINT", DXGI_FORMAT_R16G16B16A16_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16B16A16_SNORM", DXGI_FORMAT_R16G16B16A16_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16B16A16_SINT", DXGI_FORMAT_R16G16B16A16_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32_TYPELESS", DXGI_FORMAT_R32G32_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32_FLOAT", DXGI_FORMAT_R32G32_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32_UINT", DXGI_FORMAT_R32G32_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G32_SINT", DXGI_FORMAT_R32G32_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32G8X24_TYPELESS", DXGI_FORMAT_R32G8X24_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_D32_FLOAT_S8X24_UINT", DXGI_FORMAT_D32_FLOAT_S8X24_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS", DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_X32_TYPELESS_G8X24_UINT", DXGI_FORMAT_X32_TYPELESS_G8X24_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R10G10B10A2_TYPELESS", DXGI_FORMAT_R10G10B10A2_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R10G10B10A2_UNORM", DXGI_FORMAT_R10G10B10A2_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R10G10B10A2_UINT", DXGI_FORMAT_R10G10B10A2_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R11G11B10_FLOAT", DXGI_FORMAT_R11G11B10_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8B8A8_TYPELESS", DXGI_FORMAT_R8G8B8A8_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8B8A8_UNORM", DXGI_FORMAT_R8G8B8A8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8B8A8_UNORM_SRGB", DXGI_FORMAT_R8G8B8A8_UNORM_SRGB),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8B8A8_UINT", DXGI_FORMAT_R8G8B8A8_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8B8A8_SNORM", DXGI_FORMAT_R8G8B8A8_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8B8A8_SINT", DXGI_FORMAT_R8G8B8A8_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16_TYPELESS", DXGI_FORMAT_R16G16_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16_FLOAT", DXGI_FORMAT_R16G16_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16_UNORM", DXGI_FORMAT_R16G16_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16_UINT", DXGI_FORMAT_R16G16_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16_SNORM", DXGI_FORMAT_R16G16_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16G16_SINT", DXGI_FORMAT_R16G16_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32_TYPELESS", DXGI_FORMAT_R32_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_D32_FLOAT", DXGI_FORMAT_D32_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32_FLOAT", DXGI_FORMAT_R32_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32_UINT", DXGI_FORMAT_R32_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R32_SINT", DXGI_FORMAT_R32_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R24G8_TYPELESS", DXGI_FORMAT_R24G8_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_D24_UNORM_S8_UINT", DXGI_FORMAT_D24_UNORM_S8_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R24_UNORM_X8_TYPELESS", DXGI_FORMAT_R24_UNORM_X8_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_X24_TYPELESS_G8_UINT", DXGI_FORMAT_X24_TYPELESS_G8_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8_TYPELESS", DXGI_FORMAT_R8G8_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8_UNORM", DXGI_FORMAT_R8G8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8_UINT", DXGI_FORMAT_R8G8_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8_SNORM", DXGI_FORMAT_R8G8_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8_SINT", DXGI_FORMAT_R8G8_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16_TYPELESS", DXGI_FORMAT_R16_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16_FLOAT", DXGI_FORMAT_R16_FLOAT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_D16_UNORM", DXGI_FORMAT_D16_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16_UNORM", DXGI_FORMAT_R16_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16_UINT", DXGI_FORMAT_R16_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16_SNORM", DXGI_FORMAT_R16_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R16_SINT", DXGI_FORMAT_R16_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8_TYPELESS", DXGI_FORMAT_R8_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8_UNORM", DXGI_FORMAT_R8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8_UINT", DXGI_FORMAT_R8_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8_SNORM", DXGI_FORMAT_R8_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8_SINT", DXGI_FORMAT_R8_SINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_A8_UNORM", DXGI_FORMAT_A8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R1_UNORM", DXGI_FORMAT_R1_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R9G9B9E5_SHAREDEXP", DXGI_FORMAT_R9G9B9E5_SHAREDEXP),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R8G8_B8G8_UNORM", DXGI_FORMAT_R8G8_B8G8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_G8R8_G8B8_UNORM", DXGI_FORMAT_G8R8_G8B8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC1_TYPELESS", DXGI_FORMAT_BC1_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC1_UNORM", DXGI_FORMAT_BC1_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC1_UNORM_SRGB", DXGI_FORMAT_BC1_UNORM_SRGB),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC2_TYPELESS", DXGI_FORMAT_BC2_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC2_UNORM", DXGI_FORMAT_BC2_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC2_UNORM_SRGB", DXGI_FORMAT_BC2_UNORM_SRGB),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC3_TYPELESS", DXGI_FORMAT_BC3_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC3_UNORM", DXGI_FORMAT_BC3_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC3_UNORM_SRGB", DXGI_FORMAT_BC3_UNORM_SRGB),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC4_TYPELESS", DXGI_FORMAT_BC4_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC4_UNORM", DXGI_FORMAT_BC4_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC4_SNORM", DXGI_FORMAT_BC4_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC5_TYPELESS", DXGI_FORMAT_BC5_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC5_UNORM", DXGI_FORMAT_BC5_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC5_SNORM", DXGI_FORMAT_BC5_SNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B5G6R5_UNORM", DXGI_FORMAT_B5G6R5_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B5G5R5A1_UNORM", DXGI_FORMAT_B5G5R5A1_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B8G8R8A8_UNORM", DXGI_FORMAT_B8G8R8A8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B8G8R8X8_UNORM", DXGI_FORMAT_B8G8R8X8_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM", DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B8G8R8A8_TYPELESS", DXGI_FORMAT_B8G8R8A8_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B8G8R8A8_UNORM_SRGB", DXGI_FORMAT_B8G8R8A8_UNORM_SRGB),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B8G8R8X8_TYPELESS", DXGI_FORMAT_B8G8R8X8_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B8G8R8X8_UNORM_SRGB", DXGI_FORMAT_B8G8R8X8_UNORM_SRGB),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC6H_TYPELESS", DXGI_FORMAT_BC6H_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC6H_UF16", DXGI_FORMAT_BC6H_UF16),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC6H_SF16", DXGI_FORMAT_BC6H_SF16),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC7_TYPELESS", DXGI_FORMAT_BC7_TYPELESS),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC7_UNORM", DXGI_FORMAT_BC7_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_BC7_UNORM_SRGB", DXGI_FORMAT_BC7_UNORM_SRGB),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_AYUV", DXGI_FORMAT_AYUV),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_Y410", DXGI_FORMAT_Y410),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_Y416", DXGI_FORMAT_Y416),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_NV12", DXGI_FORMAT_NV12),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_P010", DXGI_FORMAT_P010),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_P016", DXGI_FORMAT_P016),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_420_OPAQUE", DXGI_FORMAT_420_OPAQUE),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_YUY2", DXGI_FORMAT_YUY2),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_Y210", DXGI_FORMAT_Y210),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_Y216", DXGI_FORMAT_Y216),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_NV11", DXGI_FORMAT_NV11),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_AI44", DXGI_FORMAT_AI44),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_IA44", DXGI_FORMAT_IA44),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_P8", DXGI_FORMAT_P8),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_A8P8", DXGI_FORMAT_A8P8),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_B4G4R4A4_UNORM", DXGI_FORMAT_B4G4R4A4_UNORM),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_P208,", DXGI_FORMAT_P208),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_V208", DXGI_FORMAT_V208),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_V408", DXGI_FORMAT_V408),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE", DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE", DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Entrie("DXGI_FORMAT_FORCE_UINT", DXGI_FORMAT_FORCE_UINT),
    ::MGX::Serialisation::StringHashTable<DXGI_FORMAT>::Terminator()
};

::MGX::Serialisation::StringHashTable<D3D12_INPUT_CLASSIFICATION> MGX::Core::Serialisation::PipelineState_helpers::s_D3D12_INPUT_CLASSIFICATION_table[] =
{
    ::MGX::Serialisation::StringHashTable<D3D12_INPUT_CLASSIFICATION>::Entrie("D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA", D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA),
    ::MGX::Serialisation::StringHashTable<D3D12_INPUT_CLASSIFICATION>::Entrie("D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA", D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA),
    ::MGX::Serialisation::StringHashTable<D3D12_INPUT_CLASSIFICATION>::Terminator()
};

void MGX::Core::Serialisation::PipelineState_helpers::GennerateHashValues() noexcept
{
    // Gennerate hash values
    ::MGX::Serialisation::StringHashTableOps<D3D12_BLEND>::GennerateHashs(s_D3D12_BLEND_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_BLEND_OP>::GennerateHashs(s_D3D12_BLEND_OP_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_LOGIC_OP>::GennerateHashs(s_D3D12_LOGIC_OP_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_FILL_MODE>::GennerateHashs(s_D3D12_FILL_MODE_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_CULL_MODE>::GennerateHashs(s_D3D12_CULL_MODE_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_CONSERVATIVE_RASTERIZATION_MODE>::GennerateHashs(s_D3D12_CONSERVATIVE_RASTERIZATION_MODE_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_DEPTH_WRITE_MASK>::GennerateHashs(s_D3D12_DEPTH_WRITE_MASK_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_COMPARISON_FUNC>::GennerateHashs(s_D3D12_COMPARISON_FUNC_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_STENCIL_OP>::GennerateHashs(s_D3D12_STENCIL_OP_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>::GennerateHashs(s_D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_PRIMITIVE_TOPOLOGY_TYPE>::GennerateHashs(s_D3D12_PRIMITIVE_TOPOLOGY_TYPE_table);
    ::MGX::Serialisation::StringHashTableOps<DXGI_FORMAT>::GennerateHashs(s_DXGI_FORMAT_table);
    ::MGX::Serialisation::StringHashTableOps<D3D12_INPUT_CLASSIFICATION>::GennerateHashs(s_D3D12_INPUT_CLASSIFICATION_table);
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerGfxPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC* ptrStateDesc, const char** ppPsoName, tinyxml2::XMLElement* ptrRootElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Validate type
    auto attPsDescType = ptrRootElement->FindAttribute("Type");
    if (attPsDescType && strcmp(attPsDescType->Value(), "graphics") == 0)
    {
        // Get name attribute
        auto attPsName = ptrRootElement->FindAttribute("Name");
        if (attPsName)
        {
            // Set name
            *ppPsoName = StrAllocateCopyA(ptrAllocator, attPsName->Value());
        }

        // Default shaders
        ptrStateDesc->PS = { 0,0 };
        ptrStateDesc->VS = { 0,0 };
        ptrStateDesc->GS = { 0,0 };
        ptrStateDesc->HS = { 0,0 };
        ptrStateDesc->DS = { 0,0 };

        // Read all shaders
        for (MGX_TINYXML_FOREACH(ptrRootElement, ptrShaderElement, "Shader"))
        {
            // Search attributes
            auto attShaderType = ptrShaderElement->FindAttribute("Type");
            auto attShaderPath = ptrShaderElement->FindAttribute("Path");

            // Check if entry is valid
            if (attShaderPath && attShaderType)
            {
                // Process shader
                const char* shaderType = attShaderType->Value();
                const char* shaderPath = attShaderPath->Value();
                DSerLoadShader(ptrStateDesc, shaderType, shaderPath, ptrAllocator);
            }
        }

        // Find and read StreamOutput
        auto ptrSOElement = ptrRootElement->FirstChildElement("StreamOutput");
        DSerLoadStreamOutput(&ptrStateDesc->StreamOutput, ptrSOElement, ptrAllocator);

        // Find and read BlendState
        auto ptrBlendElement = ptrRootElement->FirstChildElement("BlendState");
        DSerLoadBlendState(&ptrStateDesc->BlendState, &ptrStateDesc->SampleMask, ptrBlendElement, ptrAllocator);

        // Find and read RasterizerState
        auto ptrRasterizerStateElement = ptrRootElement->FirstChildElement("RasterizerState");
        DSerLoadRasterizerState(&ptrStateDesc->RasterizerState, ptrRasterizerStateElement, ptrAllocator);

        // Find and read DepthStencilState
        auto ptrDepthStencilStateElement = ptrRootElement->FirstChildElement("DepthStencilState");
        DSerDepthStencilState(&ptrStateDesc->DepthStencilState, ptrDepthStencilStateElement, ptrAllocator);

        // Find and read InputLayout
        auto ptrInputLayoutElement = ptrRootElement->FirstChildElement("InputLayout");
        DSerInputLayouts(&ptrStateDesc->InputLayout, ptrInputLayoutElement, ptrAllocator);

        // Find and read IBStripCutValue
        auto ptrIBStripCutValueElement = ptrRootElement->FirstChildElement("IBStripCutValue");
        if (ptrIBStripCutValueElement)
        {
            Util::XML::QueryOrDefault(ptrIBStripCutValueElement->FindAttribute("Value"), &ptrStateDesc->IBStripCutValue, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED, s_D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_table);
        }
        else
        {
            ptrStateDesc->IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        }

        // Find and read PrimitiveTopologyType
        auto ptrPrimitiveTopologyTypeElement = ptrRootElement->FirstChildElement("PrimitiveTopologyType");
        if (ptrPrimitiveTopologyTypeElement)
        {
            Util::XML::QueryOrDefault(ptrPrimitiveTopologyTypeElement->FindAttribute("Value"), &ptrStateDesc->PrimitiveTopologyType, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, s_D3D12_PRIMITIVE_TOPOLOGY_TYPE_table);
        }
        else
        {
            ptrStateDesc->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        }

        // Find and read RenderTargets
        auto ptrRenderTargetsElement = ptrRootElement->FirstChildElement("RenderTargets");
        DSerRenderTargets(&ptrStateDesc->DSVFormat, ptrStateDesc->RTVFormats, &ptrStateDesc->NumRenderTargets, ptrRenderTargetsElement, ptrAllocator);

        // Find and read SampleDesc
        auto ptrSampleDescElement = ptrRootElement->FirstChildElement("SampleDesc");
        DSerSampleDesc(&ptrStateDesc->SampleDesc, ptrSampleDescElement, ptrAllocator);

        // Ok
        return true;
    }
    
    // Fallback invalid node
    return false;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerLoadShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC* ptrStateDesc, const char* shaderType, const char* shaderPath, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    bool result = false;

    // Check type value
    if (strlen(shaderType) == 2)
    {
        // Pach shader code
        const UINT16 shadeCode = (shaderType[0] << 8) | shaderType[1];

        // Find target shader
        D3D12_SHADER_BYTECODE* ptrBytecodeDesc;
        switch (shadeCode)
        {
            // Vertex shader
            case (('V' << 8) | 'S'):
                ptrBytecodeDesc = &ptrStateDesc->VS;
                break;

            // Pixel shader
            case (('P' << 8) | 'S'):
                ptrBytecodeDesc = &ptrStateDesc->PS;
                break;

            // Hull shader
            case (('H' << 8) | 'S'):
                ptrBytecodeDesc = &ptrStateDesc->HS;
                break;

            // Domain shader
            case (('D' << 8) | 'S'):
                ptrBytecodeDesc = &ptrStateDesc->DS;
                break;

            // Geometry shader
            case (('G' << 8) | 'S'):
                ptrBytecodeDesc = &ptrStateDesc->GS;
                break;

            /*
            // Amplification shader
            case (('A' << 8) | 'S'):
                ptrBytecodeDesc = &ptrStateDesc->AS;
                break;

            // Mesh shader
            case (('M' << 8) | 'S'):
                ptrBytecodeDesc = &ptrStateDesc->MS;
                break;
            */

            // Default 
            default:
                ptrBytecodeDesc = nullptr;
        }

        // Read shader
        if (ptrBytecodeDesc)
        {
            // Default
            ptrBytecodeDesc->BytecodeLength = 0;
            ptrBytecodeDesc->pShaderBytecode = nullptr;

            // Build filename
            CHAR fileName[4096];
            strcpy_s<4096>(fileName, shaderPath);
            strcat_s<4096>(fileName, ".cso");

            // Open file
            HANDLE hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // Get size and allocate buffer
                DWORD shaderSize = GetFileSize(hFile, nullptr);
                if (shaderSize)
                {
                    // Allocate memory
                    void* shaderMemory = ptrAllocator->Allocate(shaderSize);
                    if (shaderMemory)
                    {
                        // Read memory
                        if (ReadFile(hFile, shaderMemory, shaderSize, 0, NULL))
                        {
                            // Set information
                            ptrBytecodeDesc->BytecodeLength = shaderSize;
                            ptrBytecodeDesc->pShaderBytecode = shaderMemory;

                            // OK
                            result = true;
                        }
                    }
                }

                CloseHandle(hFile);
            }
        }
    }

    return result;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerLoadStreamOutput(D3D12_STREAM_OUTPUT_DESC* ptrSODesc, tinyxml2::XMLElement* ptrSOElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Default
    *ptrSODesc = { nullptr, 0, nullptr, 0, 0 };

    // Check so element
    if (ptrSOElement)
    {
        // Read Attribute value
        Util::XML::QueryOrDefault(ptrSOElement->FindAttribute("RasterizedStream"), &ptrSODesc->RasterizedStream, 0U);

        // Count Declaration(s)
        unsigned int numDeclarations = 0;
        for (MGX_TINYXML_FOREACH(ptrSOElement, p, "Declaration")) 
            numDeclarations++;

        // Aquire declarations
        if (numDeclarations)
        {
            // Allocate memory for declarations
            D3D12_SO_DECLARATION_ENTRY* const ptrEntries = ptrAllocator->TAllocate<D3D12_SO_DECLARATION_ENTRY>(numDeclarations);
            if (ptrEntries)
            {
                // Deserialize SoEntryies
                D3D12_SO_DECLARATION_ENTRY* ptrHead = ptrEntries;
                for (MGX_TINYXML_FOREACH(ptrSOElement, ptrDeclElement, "Declaration"))
                    DSerLoadSOEntry(ptrHead++, ptrDeclElement, ptrAllocator);

                // Fill descritor
                ptrSODesc->pSODeclaration = ptrEntries;
                ptrSODesc->NumEntries = numDeclarations;
            }
        }

        // Count Strides(s)
        unsigned int numStrides = 0;
        for (MGX_TINYXML_FOREACH(ptrSOElement, p, "Stride"))
            numStrides++;
        
        // Aquire strides
        if (numStrides)
        {
            // Allocate memory for strides
            UINT32* const ptrStrides = ptrAllocator->TAllocate<UINT32>(numStrides);
            if (ptrStrides)
            {
                // Deserialize strides
                UINT* ptrHead = ptrStrides;
                for (MGX_TINYXML_FOREACH(ptrSOElement, ptrStrideElement, "Stride")) 
                    Util::XML::QueryOrDefault(ptrStrideElement->FindAttribute("Value"), (ptrHead++), 0U);

                // Fill descriptor
                ptrSODesc->pBufferStrides = ptrStrides;
                ptrSODesc->NumStrides = numStrides;
            }
        }
    }

    return true;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerLoadSOEntry(D3D12_SO_DECLARATION_ENTRY* ptrSOEntrie, tinyxml2::XMLElement* ptrSOEntrieElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Default
    *ptrSOEntrie = {0, nullptr, 0, 0, 0, 0};

    // Check element
    if (ptrSOEntrieElement)
    {
        // Extract strait forward attributes
        Util::XML::QueryOrDefault(ptrSOEntrieElement->FindAttribute("Stream"), &ptrSOEntrie->Stream, 0U);
        Util::XML::QueryOrDefault(ptrSOEntrieElement->FindAttribute("SemanticIndex"), &ptrSOEntrie->SemanticIndex, 0U);
        Util::XML::QueryOrDefault<BYTE>(ptrSOEntrieElement->FindAttribute("StartComponent"), &ptrSOEntrie->StartComponent, 0);
        Util::XML::QueryOrDefault<BYTE>(ptrSOEntrieElement->FindAttribute("ComponentCount"), &ptrSOEntrie->ComponentCount, 0);
        Util::XML::QueryOrDefault<BYTE>(ptrSOEntrieElement->FindAttribute("OutputSlot"), &ptrSOEntrie->OutputSlot, 0);

        // Extract string attribute
        auto attSemanticName = ptrSOEntrieElement->FindAttribute("SemanticName");
        if (attSemanticName)
        {
            // Get length of string
            size_t strLen = strlen(attSemanticName->Value());
            if (strLen)
            {
                // Allocate string
                char* strBuffer = ptrAllocator->TAllocate<char>(strLen + 1);
                if (strBuffer)
                {
                    // Copy string
                    strcpy_s(strBuffer, strLen + 1, attSemanticName->Value());

                    // Set string
                    ptrSOEntrie->SemanticName = strBuffer;

                    // OK
                    return true;
                }
            }
        }
    }

    // Fallback fail
    return false;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerLoadBlendState(D3D12_BLEND_DESC* ptrBlendDesc, UINT32* ptrSampleMask, tinyxml2::XMLElement* ptrBlendDescElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Default
    ptrBlendDesc->AlphaToCoverageEnable = false;
    ptrBlendDesc->IndependentBlendEnable = false;
    ptrBlendDesc->RenderTarget[0] = {false, false, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL };

    if (ptrBlendDescElement)
    {
        // Read naive attributes
        Util::XML::QueryOrDefault(ptrBlendDescElement->FindAttribute("AlphaToCoverageEnable"), (bool*)&ptrBlendDesc->AlphaToCoverageEnable, false);
        Util::XML::QueryOrDefault(ptrBlendDescElement->FindAttribute("IndependentBlendEnable"), (bool*)&ptrBlendDesc->IndependentBlendEnable, false);

        // Read sample mask
        auto attSampleMask = ptrBlendDescElement->FindAttribute("SampleMask");
        if (attSampleMask)
        {
            // Convert to integer
            std::stringstream ss;
            ss << std::hex << attSampleMask->Value();
            ss >> *ptrSampleMask;
        }
        else 
        {
            *ptrSampleMask = 0xFFFFFFFF;
        }

        // Aquire rt structs
        int numDeclarations = 0;
        for (MGX_TINYXML_FOREACH(ptrBlendDescElement, ptrBlendRtDescElement, "Blend"))
        {
            // Only if range is valid
            if (numDeclarations < 8)
            {
                if (!DSerLoadBlendStateRT(&ptrBlendDesc->RenderTarget[numDeclarations], ptrBlendRtDescElement, ptrAllocator))
                    numDeclarations--;
            }

            // Increment
            numDeclarations++;
        }

        // Return result
        return numDeclarations < 1 && numDeclarations < 9;
    }

    // Default failed
    return false;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerLoadBlendStateRT(D3D12_RENDER_TARGET_BLEND_DESC* ptrBlendRtDesc, tinyxml2::XMLElement* ptrBlendRtDescElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Naive attributes
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("BlendEnable"), (bool*)&ptrBlendRtDesc->BlendEnable, false);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("LogicOpEnable"), (bool*)&ptrBlendRtDesc->LogicOpEnable, false);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("SrcBlend"), &ptrBlendRtDesc->SrcBlend, D3D12_BLEND_ONE, s_D3D12_BLEND_table);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("DestBlend"), &ptrBlendRtDesc->DestBlend, D3D12_BLEND_ZERO, s_D3D12_BLEND_table);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("BlendOp"), &ptrBlendRtDesc->BlendOp, D3D12_BLEND_OP_ADD, s_D3D12_BLEND_OP_table);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("SrcBlendAlpha"), &ptrBlendRtDesc->SrcBlendAlpha, D3D12_BLEND_ONE, s_D3D12_BLEND_table);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("DestBlendAlpha"), &ptrBlendRtDesc->DestBlendAlpha, D3D12_BLEND_ZERO, s_D3D12_BLEND_table);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("BlendOpAlpha"), &ptrBlendRtDesc->BlendOpAlpha, D3D12_BLEND_OP_ADD, s_D3D12_BLEND_OP_table);
    Util::XML::QueryOrDefault(ptrBlendRtDescElement->FindAttribute("LogicOp"), &ptrBlendRtDesc->LogicOp, D3D12_LOGIC_OP_NOOP, s_D3D12_LOGIC_OP_table);

    // RenderTargetWriteMask
    auto attRenderTargetWriteMask = ptrBlendRtDescElement->FindAttribute("RenderTargetWriteMask");
    if (attRenderTargetWriteMask)
    {
        // Clear random contained value
        ptrBlendRtDesc->RenderTargetWriteMask = 0x0;

        // Itterate over string chars
        for (const char* ptrChar = attRenderTargetWriteMask->Value(); *ptrChar != '\0'; ptrChar++)
        {
            // Evalue char values
            switch (*ptrChar)
            {
                // Red
                case 'R':
                    ptrBlendRtDesc->RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_RED;
                    break;
                // Green
                case 'G':
                    ptrBlendRtDesc->RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_GREEN;
                    break;
                // Blue
                case 'B':
                    ptrBlendRtDesc->RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_BLUE;
                    break;
                // Alpha
                case 'A':
                    ptrBlendRtDesc->RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
                    break;
            }
        }
    }
    else
    {
        // Default
        ptrBlendRtDesc->RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    }

    // Passed
    return true;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerLoadRasterizerState(D3D12_RASTERIZER_DESC* ptrRasterizerState, tinyxml2::XMLElement* ptrRasterizerStateElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    *ptrRasterizerState = { D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, FALSE, 0, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0, D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF };

    if (ptrRasterizerStateElement)
    {
        // Load values
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("FillMode"), &ptrRasterizerState->FillMode, D3D12_FILL_MODE_SOLID, s_D3D12_FILL_MODE_table);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("CullMode"), &ptrRasterizerState->CullMode, D3D12_CULL_MODE_BACK, s_D3D12_CULL_MODE_table);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("FrontCounterClockwise"), (bool*)&ptrRasterizerState->FrontCounterClockwise, false);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("DepthBias"), &ptrRasterizerState->DepthBias, 0);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("DepthBiasClamp"), &ptrRasterizerState->DepthBiasClamp, 0.0f);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("SlopeScaledDepthBias"), &ptrRasterizerState->SlopeScaledDepthBias, 0.0f);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("DepthClipEnable"), (bool*)&ptrRasterizerState->DepthClipEnable, true);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("MultisampleEnable"), (bool*)&ptrRasterizerState->MultisampleEnable, false);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("AntialiasedLineEnable"), (bool*)&ptrRasterizerState->AntialiasedLineEnable, false);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("ForcedSampleCount"), &ptrRasterizerState->ForcedSampleCount, 0U);
        Util::XML::QueryOrDefault(ptrRasterizerStateElement->FindAttribute("ConservativeRaster"), &ptrRasterizerState->ConservativeRaster, D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF, s_D3D12_CONSERVATIVE_RASTERIZATION_MODE_table);

        // Passed
        return true;
    }

    // Invalid args
    return false;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerDepthStencilState(D3D12_DEPTH_STENCIL_DESC* ptrDepthStencilState, tinyxml2::XMLElement* ptrDepthStencilStateElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    *ptrDepthStencilState = {
        TRUE, D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC_LESS, FALSE, D3D12_DEFAULT_STENCIL_READ_MASK, D3D12_DEFAULT_STENCIL_WRITE_MASK, 
        { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS },
        { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS }
    };

    if (ptrDepthStencilStateElement)
    {
        // Load naive values
        Util::XML::QueryOrDefault(ptrDepthStencilStateElement->FindAttribute("DepthEnable"), (bool*)&ptrDepthStencilState->DepthEnable, true);
        Util::XML::QueryOrDefault(ptrDepthStencilStateElement->FindAttribute("DepthWriteMask"), &ptrDepthStencilState->DepthWriteMask, D3D12_DEPTH_WRITE_MASK_ALL, s_D3D12_DEPTH_WRITE_MASK_table);
        Util::XML::QueryOrDefault(ptrDepthStencilStateElement->FindAttribute("DepthFunc"), &ptrDepthStencilState->DepthFunc, D3D12_COMPARISON_FUNC_LESS, s_D3D12_COMPARISON_FUNC_table);
        Util::XML::QueryOrDefault(ptrDepthStencilStateElement->FindAttribute("StencilEnable"), (bool*)&ptrDepthStencilState->StencilEnable, false);
        
        // Stencil read mask
        auto attStencilReadMask = ptrDepthStencilStateElement->FindAttribute("StencilReadMask");
        if (attStencilReadMask)
        {
            std::stringstream ss;
            ss << std::hex << attStencilReadMask->Value();
            ss >> ptrDepthStencilState->StencilReadMask;
        }

        // Stencil write mask
        auto attStencilWriteMask = ptrDepthStencilStateElement->FindAttribute("StencilWriteMask");
        if (attStencilWriteMask)
        {
            std::stringstream ss;
            ss << std::hex << attStencilWriteMask->Value();
            ss >> ptrDepthStencilState->StencilWriteMask;
        }

        // Font face
        auto ptrFrontFaceElement = ptrDepthStencilStateElement->FirstChildElement("FrontFace");
        if (ptrFrontFaceElement)
        {
            DSerStencilOp(&ptrDepthStencilState->FrontFace, ptrFrontFaceElement, ptrAllocator);
        }

        // Back face
        auto ptrBackFaceElement = ptrDepthStencilStateElement->FirstChildElement("BackFace");
        if (ptrBackFaceElement)
        {
            DSerStencilOp(&ptrDepthStencilState->BackFace, ptrBackFaceElement, ptrAllocator);
        }
    }
    
    // Invalid args
    return false;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerStencilOp(D3D12_DEPTH_STENCILOP_DESC* ptrStencilOp, tinyxml2::XMLElement* ptrStencilOpElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Load values
    Util::XML::QueryOrDefault(ptrStencilOpElement->FindAttribute("StencilFailOp"), &ptrStencilOp->StencilFailOp, D3D12_STENCIL_OP_KEEP, s_D3D12_STENCIL_OP_table);
    Util::XML::QueryOrDefault(ptrStencilOpElement->FindAttribute("StencilDepthFailOp"), &ptrStencilOp->StencilDepthFailOp, D3D12_STENCIL_OP_KEEP, s_D3D12_STENCIL_OP_table);
    Util::XML::QueryOrDefault(ptrStencilOpElement->FindAttribute("StencilPassOp"), &ptrStencilOp->StencilPassOp, D3D12_STENCIL_OP_KEEP, s_D3D12_STENCIL_OP_table);
    Util::XML::QueryOrDefault(ptrStencilOpElement->FindAttribute("StencilFunc"), &ptrStencilOp->StencilFunc, D3D12_COMPARISON_FUNC_ALWAYS, s_D3D12_COMPARISON_FUNC_table);

    // Passed
    return true;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerSampleDesc(DXGI_SAMPLE_DESC* ptrSampleDesc, tinyxml2::XMLElement* ptrSampleDescElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    *ptrSampleDesc = { 1, 0 };

    if (ptrSampleDesc)
    {
        // Read values
        Util::XML::QueryOrDefault(ptrSampleDescElement->FindAttribute("Count"), &ptrSampleDesc->Count, 1U);
        Util::XML::QueryOrDefault(ptrSampleDescElement->FindAttribute("Quality"), &ptrSampleDesc->Quality, 0U);

        // Ok
        return true;
    }

    // Invalid args
    return false;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerInputLayouts(D3D12_INPUT_LAYOUT_DESC* ptrInputLayout, tinyxml2::XMLElement* ptrInputLayoutsElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    *ptrInputLayout = {nullptr, 0};

    // Check
    if (ptrInputLayoutsElement)
    {
        // Count layouts
        unsigned int numLayouts = 0;
        for (MGX_TINYXML_FOREACH(ptrInputLayoutsElement, p, "InputElement"))
            numLayouts++;

        // Allocate memory
        D3D12_INPUT_ELEMENT_DESC* ptrDescs = ptrAllocator->TAllocate<D3D12_INPUT_ELEMENT_DESC>(numLayouts);
        if (ptrDescs)
        {
            // Reset num layouts
            numLayouts = 0;

            // Evaluate layouts
            for (MGX_TINYXML_FOREACH(ptrInputLayoutsElement, ptrInputElementElement, "InputElement"))
            {
                if (DSerInputLayoutElement(&ptrDescs[numLayouts], ptrInputElementElement, ptrAllocator))
                    numLayouts++;
            }

            // Set
            ptrInputLayout->NumElements = numLayouts;
            ptrInputLayout->pInputElementDescs = ptrDescs;

            // Passed
            return true;
        }
    }

    // Fallback
    return false;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerInputLayoutElement(D3D12_INPUT_ELEMENT_DESC* ptrInputLayoutElement, tinyxml2::XMLElement* ptrInputLayoutElementElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Find strait forward
    Util::XML::QueryOrDefault(ptrInputLayoutElementElement->FindAttribute("SemanticIndex"), &ptrInputLayoutElement->SemanticIndex, 0U);
    Util::XML::QueryOrDefault(ptrInputLayoutElementElement->FindAttribute("Format"), &ptrInputLayoutElement->Format, DXGI_FORMAT_UNKNOWN, s_DXGI_FORMAT_table);
    Util::XML::QueryOrDefault(ptrInputLayoutElementElement->FindAttribute("InputSlot"), &ptrInputLayoutElement->InputSlot, 0U);
    Util::XML::QueryOrDefault(ptrInputLayoutElementElement->FindAttribute("AlignedByteOffset"), &ptrInputLayoutElement->AlignedByteOffset, 0xffffffffU);
    Util::XML::QueryOrDefault(ptrInputLayoutElementElement->FindAttribute("InputSlotClass"), &ptrInputLayoutElement->InputSlotClass, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, s_D3D12_INPUT_CLASSIFICATION_table);
    Util::XML::QueryOrDefault(ptrInputLayoutElementElement->FindAttribute("InstanceDataStepRate"), &ptrInputLayoutElement->InstanceDataStepRate, 0U);

    // Find text
    auto attSemanticName = ptrInputLayoutElementElement->FindAttribute("SemanticName");
    if (attSemanticName)
    {
        // Copy name
        ptrInputLayoutElement->SemanticName = StrAllocateCopyA(ptrAllocator, attSemanticName->Value());
    }
    else
    {
        ptrInputLayoutElement->SemanticName = nullptr;
    }

    // Text ok?
    return ptrInputLayoutElement->SemanticName != nullptr;
}

bool MGX::Core::Serialisation::PipelineState_helpers::DSerRenderTargets(DXGI_FORMAT* ptrDSVFormat, DXGI_FORMAT* ptrRTVFormats, UINT* ptrNumRTV, tinyxml2::XMLElement* ptrRenderTargetsElement, Allocator::StackMemoryAllocator* ptrAllocator) noexcept
{
    // Check
    if (!ptrRenderTargetsElement)
    {
        return false;
    }

    // Get DSV Format
    Util::XML::QueryOrDefault(ptrRenderTargetsElement->FindAttribute("DSVFormat"), ptrDSVFormat, DXGI_FORMAT_UNKNOWN, s_DXGI_FORMAT_table);

    // Get RTV Formats
    unsigned int rtvFormatCount = 0;
    for (MGX_TINYXML_FOREACH(ptrRenderTargetsElement, ptrRenderTargetElement, "RenderTarget"))
    {
        // Aquire format
        if (rtvFormatCount < 8)
        {
            Util::XML::QueryOrDefault(ptrRenderTargetElement->FindAttribute("RTVFormat"), &ptrRTVFormats[rtvFormatCount], DXGI_FORMAT_UNKNOWN, s_DXGI_FORMAT_table);
        }

        // Increment 
        rtvFormatCount++;
    }

    // Set count
    *ptrNumRTV = std::min<UINT>(8, rtvFormatCount);

    // Compute if result was positive
    return rtvFormatCount > 0 && rtvFormatCount <= 8;
}
