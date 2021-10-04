#include "ModernGX.h"
#include "SerPipelineState.h"

#include <ModernGX/Core/Serialisation/_internal_SerPipelineState.h>

void MGX::Core::Serialisation::PipelineState::Init() noexcept
{
    PipelineState_helpers::GennerateHashValues();
}

bool MGX::Core::Serialisation::PipelineState::LoadPipelineStateFromDisk(ID3D12RootSignature** ppRootSignature, ID3D12PipelineState** ppPipelineState, const wchar_t* xmlSubFilePath, IAllocator* ptrAllocator) noexcept
{
    // Create base allocator
    // 2048 * 64 Bytes --> 128K
    Allocator::StackMemoryAllocator workStackAllocator(2048, 64, ptrAllocator);

    // Module dir
    WCHAR moduleDir[2048];
    GetModuleFileNameW(GetModuleHandle(NULL), moduleDir, 2048);
    wcsrchr(moduleDir, '\\')[1] = 0x00;

    // Work dir
    WCHAR workDir[2048];
    GetCurrentDirectoryW(2048, workDir);
    wcscat_s<2048>(workDir, L"\\");

    // Open file
    Util::File xmlFile = Util::File::Open(xmlSubFilePath, workDir, moduleDir);
    if (xmlFile && xmlFile.Open())
    {
        // Allocate memory
        UINT64 size = xmlFile.Size();
        char* fileContent = workStackAllocator.TAllocate<char>(size);
        if (xmlFile.ReadT<char>(fileContent, size))
        {
            // Create xml document
            tinyxml2::XMLDocument doc;
            if (doc.Parse(fileContent, size) == tinyxml2::XMLError::XML_SUCCESS)
            {
                // Find root node
                auto ptrRootElement = doc.FirstChildElement("PipelineConfig");
                if (ptrRootElement)
                {
                    // Parse document
                    const char* psoName = nullptr;
                    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
                    if (PipelineState_helpers::DSerGfxPipelineState(&desc, &psoName, ptrRootElement, &workStackAllocator))
                    {
                        // ...
                    }
                    int i = 0;
                }
            }
        }
    }

    // Failed
    return false;
}
