#include "ModernGX.h"
#include "GPUDevice.h"

MGX::Core::GPU::Device::Device() {
    // Create device
    MGX_EVALUATE_HRESULT("D3D12CreateDevice(...)", D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_ptrBase)));

    // Get debug device
    #ifdef _DEBUG
    m_ptrBase.queryInterface(m_ptrDebugDevice);
    #endif
}

MGX::Core::GPU::Device::Device(LUID luid) {
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
    m_ptrBase.queryInterface(m_ptrDebugDevice);
    #endif 
}

MGX::Core::GPU::Device::~Device() noexcept {
    // Check if this was the last object of this type (ref count == 1 because Debug Device and Device have same ref count)
    auto ref = m_ptrBase.release();
    #ifdef _DEBUG
    if (ref == 1) {
        // RLDO on debug
        OutputDebugString(L"DirectX 12 is reporting live device objects. One device ist totaly normal (this is the device producing that output)\n");
        m_ptrDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
    }

    // Release debug as well
    m_ptrDebugDevice.release();
    #endif
}

DXGI_ADAPTER_DESC3 MGX::Core::GPU::Device::GetDescription() noexcept {
    DXGI_ADAPTER_DESC3 desc = {};
    
    // Create DXGI Factory
    ComPointer<IDXGIFactory4> factory;
    if (SUCCEEDED(CreateDXGIFactory2(NULL, IID_PPV_ARGS(&factory)))) {
        ComPointer<IDXGIAdapter4> adapter;
        if (SUCCEEDED(factory->EnumAdapterByLuid(m_ptrBase->GetAdapterLuid(), IID_PPV_ARGS(&adapter)))) {
            adapter->GetDesc3(&desc);
        }
    }

    // Return description
    return desc;
}
