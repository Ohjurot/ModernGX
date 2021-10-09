#include "ModernGX.h"
#include "GPUPipelineState.h"

MGX::Core::GPU::PipelineState::PipelineState(ID3D12Device* ptrDevice, const wchar_t* configFileName, IAllocator* ptrAllocator)
{
    // Load PSO from disk
    if (!Serialisation::PipelineState::LoadPipelineStateFromDisk(ptrDevice, &m_rootSignature, &m_pipelineState, configFileName, ptrAllocator))
    {
        throw std::exception("Failed to load pipeline state from configuraition XML file.");
    }

    // Set type
    m_type = PipelineStateType::Graffics;
}

MGX::Core::GPU::PipelineState::PipelineState(ID3D12Device* ptrDevice, ID3D12RootSignature* ptrSignature, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* gfxDescription)
{
    // Store root
    m_rootSignature = ptrSignature;
    
    // Create PSO
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateGraphicsPipelineState(...)", ptrDevice->CreateGraphicsPipelineState(gfxDescription, IID_PPV_ARGS(&m_pipelineState)));

    // Set type
    m_type = PipelineStateType::Graffics;
}

MGX::Core::GPU::PipelineState::PipelineState(ID3D12Device* ptrDevice, ID3D12RootSignature* ptrSignature, const D3D12_COMPUTE_PIPELINE_STATE_DESC* computeDescription)
{
    // Store root
    m_rootSignature = ptrSignature;

    // Create PSO
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateGraphicsPipelineState(...)", ptrDevice->CreateComputePipelineState(computeDescription, IID_PPV_ARGS(&m_pipelineState)));

    // Set type
    m_type = PipelineStateType::Compute;
}

MGX::Core::GPU::PipelineState::PipelineState(ID3D12Device* ptrDevice, const wchar_t* computeShaderFile, IAllocator* ptrAllocator)
{
    // Target data
    void* ptrTargetBlob = nullptr;
    size_t targetSize = 0;

    // Get work dir
    WCHAR workDir[1024];
    GetCurrentDirectory(1024, workDir);

    // Get bin dir
    WCHAR binDir[1024];
    GetModuleFileName(GetModuleHandle(NULL), binDir, 1024);
    auto rptr = wcsrchr(binDir, L'\\');
    if (rptr) *rptr = L'\0';

    // Load blob
    Util::File file = Util::File::Open(computeShaderFile, workDir, binDir);
    if (file.Exists() && file.Open())
    {
        // Allocate blob
        ptrTargetBlob = ptrAllocator->Allocate(file.Size());
        if (ptrTargetBlob)
        {
            // Read file
            if (!file.Read(ptrTargetBlob, file.Size()))
            {
                // Free on fail
                ptrAllocator->Free(ptrTargetBlob);
                ptrTargetBlob = nullptr;
            }
        }
    }

    // Check if dispatch is possible
    if (ptrTargetBlob)
    {
        // Construct
        __constructFromBlob(ptrDevice, this, ptrTargetBlob, targetSize);

        // Free memory
        ptrAllocator->Free(ptrTargetBlob);
    }
}

void MGX::Core::GPU::PipelineState::Bind(ID3D12GraphicsCommandList* ptrCommandList)
{
    // Bind PSO
    ptrCommandList->SetPipelineState(m_pipelineState);
    
    // Bind RootSignature
    if (m_type == PipelineStateType::Graffics)
    {
        ptrCommandList->SetGraphicsRootSignature(m_rootSignature);
    }
    else
    {
        ptrCommandList->SetComputeRootSignature(m_rootSignature);
    }
}

void MGX::Core::GPU::PipelineState::__constructFromBlob(ID3D12Device* ptrDevice, PipelineState* ptrInstance, void* buffer, size_t len)
{
    // Create root signature
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateRootSignature(...)", ptrDevice->CreateRootSignature(NULL, buffer, len, IID_PPV_ARGS(&ptrInstance->m_rootSignature)));

    // Destribe
    D3D12_COMPUTE_PIPELINE_STATE_DESC cpDesc = {};
    cpDesc.pRootSignature = ptrInstance->m_rootSignature;
    cpDesc.CS.pShaderBytecode = buffer;
    cpDesc.CS.BytecodeLength = len;
    cpDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
    cpDesc.NodeMask = NULL;
    cpDesc.CachedPSO.pCachedBlob = nullptr;
    cpDesc.CachedPSO.CachedBlobSizeInBytes = 0;

    // Create
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateComputePipelineState(...)", ptrDevice->CreateComputePipelineState(&cpDesc, IID_PPV_ARGS(&ptrInstance->m_pipelineState)));

    // Set type
    ptrInstance->m_type = PipelineStateType::Compute;
}
