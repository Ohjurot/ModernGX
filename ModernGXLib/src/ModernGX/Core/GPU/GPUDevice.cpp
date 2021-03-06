#include "ModernGX.h"
#include "GPUDevice.h"

MGX::Core::GPU::Device::Device() 
{
    // Create device
    MGX_EVALUATE_HRESULT("D3D12CreateDevice(...)", D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_ptrBase)));

    // Get debug device
    #ifdef _DEBUG
    __DebugInit();
    #endif
}

MGX::Core::GPU::Device::Device(LUID luid) 
{
    // Create DXGI Factory
    ComPointer<IDXGIFactory4> factory;
    MGX_EVALUATE_HRESULT("CreateDXGIFactory2(...) for IDXGIFactory4", CreateDXGIFactory2(NULL, IID_PPV_ARGS(&factory)));

    // Get adapter
    ComPointer<IDXGIAdapter4> adapter;
    MGX_EVALUATE_HRESULT("IDXGIFactory4->EnumAdapterByLuid(...) for IDXGIAdapter4", factory->EnumAdapterByLuid(m_ptrBase->GetAdapterLuid(), IID_PPV_ARGS(&adapter)));

    // Create device
    MGX_EVALUATE_HRESULT("D3D12CreateDevice(...)", D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_ptrBase)));

    // Get debug device
    #ifdef _DEBUG
    __DebugInit();
    #endif 
}

MGX::Core::GPU::Device::~Device() noexcept 
{
    // Check if this was the last object of this type (ref count == 1 because Debug Device and Device have same ref count)
    auto ref = m_ptrBase.release();
    #ifdef _DEBUG
    if (ref == 1 && m_ptrDebugDevice) 
    {
        // RLDO on debug
        OutputDebugString(L"DirectX 12 is reporting live device objects. One device ist totaly normal (this is the device producing that output)\n");
        m_ptrDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
    }

    // Release debug as well
    m_ptrDebugDevice.release();
    #endif
}

DXGI_ADAPTER_DESC3 MGX::Core::GPU::Device::GetDescription() noexcept 
{
    DXGI_ADAPTER_DESC3 desc = {};
    
    // Create DXGI Factory
    ComPointer<IDXGIFactory4> factory;
    if (SUCCEEDED(CreateDXGIFactory2(NULL, IID_PPV_ARGS(&factory)))) 
    {
        ComPointer<IDXGIAdapter4> adapter;
        if (SUCCEEDED(factory->EnumAdapterByLuid(m_ptrBase->GetAdapterLuid(), IID_PPV_ARGS(&adapter)))) 
        {
            adapter->GetDesc3(&desc);
        }
    }

    // Return description
    return desc;
}

MGX::Core::GPU::HeapAllocationCookie MGX::Core::GPU::Device::GetAllocationInfo(const D3D12_RESOURCE_DESC* ptrDesc) noexcept
{
    HeapAllocationCookie info = {};
    
    // Query and copy
    D3D12_RESOURCE_ALLOCATION_INFO d3dInfo = m_ptrBase->GetResourceAllocationInfo(NULL, 1, ptrDesc);
    info.size = d3dInfo.SizeInBytes;

    return info;
}

MGX::Core::GPU::FormatSupport MGX::Core::GPU::Device::CheckFormatSupport(DXGI_FORMAT format) noexcept
{
    // Query format support
    FormatSupport fmtSupport = {};
    m_ptrBase->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &fmtSupport, sizeof(FormatSupport));
    return fmtSupport;
}

MGX::Core::GPU::MSAASupport MGX::Core::GPU::Device::CheckMSAASupport(DXGI_FORMAT format, D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS flags) noexcept
{
    // Create and aliass output struct
    MSAASupport msaaSupport;

    // Loop msaa values
    UINT index = 0;
    for (UINT msaaLevel = 1; msaaLevel <= 16; msaaLevel *= 2)
    {
        // Describe info struct
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels = {};
        levels.Format = format;
        levels.SampleCount = msaaLevel;
        levels.Flags = flags;
        levels.NumQualityLevels = UINT_MAX;

        // Query value
        if (SUCCEEDED(CheckFeatureSupport<D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS>(&levels)))
        {
            msaaSupport.values[index++] = levels.NumQualityLevels;
        }
        else
        {
            msaaSupport.values[index++] = 0;
        }
    }

    return msaaSupport;
}

#ifdef _DEBUG
void MGX::Core::GPU::Device::__DebugInit() 
{
    // Create debug device and show error when failed
    if (!m_ptrBase.queryInterface(m_ptrDebugDevice)) 
    {
        OutputDebugString(L"[MGX ERROR] Failed to create ID3D12DebugDevice. Are you linking with the correct lib? Are your correctly defining MGX_DEBUG? Are you calling MGX_INIT()?\n");
    }
}
#endif

