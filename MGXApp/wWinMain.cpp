#include <ModernGX.h>
#include <ModernGX/Util/Memory.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GFXWindow.h>
#include <ModernGX/Core/GPU/GPUDevice.h>
#include <ModernGX/Core/GPU/GPUQueue.h>
#include <ModernGX/Core/GPU/GPUCommandList.h>

#include <ModernGX/Core/GPU/GPUHeap.h>
#include <ModernGX/Core/GPU/GPUResource.h>

#include <ModernGX/Core/GPU/GPUPipelineState.h>
#include <ModernGX/Core/GPU/GPURootConfiguration.h>

// Windows enable visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace MGX;

INT wWinMain_safe(HINSTANCE hInstance, PWSTR cmdArgs, INT cmdShow) 
{
    // Init MGX
    MGX_INIT();
    
    // Device
    Core::GPU::Device device; device.name(L"Main device");
    
    // Commands
    Core::GPU::CommandQueue queue(device); queue.name(L"Default direct queue");
    Core::GPU::CommandList list(device); list.name(L"Main direct command list");

    // RTV Heap
    Core::GPU::DescriptorHeap rtvDescHeap(device, Core::GPU::DescriptorHeapUsage::RenderTargetView, 16); 
    rtvDescHeap.name(L"RTV Descriptor Heap");

    // TEST
    UINT32 buffer[4] = { 0, 16, 32, 64 };
    Core::GPU::PipelineState state(device, L"Test.xml");
    Core::GPU::RootConfiguration rConf(state.GetType(),
        Core::GPU::RootConfigurationEntry::MakeRootConstant(4, buffer)
    );
    // END 

    // Create window
    Core::Window wnd(L"My Window", device, queue, rtvDescHeap.Allocate(3));

    // Prcoess window events
    while (wnd.ProcessWindowEvents()) {
        // Window resizing
        if (wnd.NeedsResizing()) {
            queue.Flush(wnd.GetBufferCount());
            wnd.ResizeNow(device);
        }

        // === Rendering ===
        // TODO: Render loop

        // === Window Compositing ===
        
        // Set Resource to RTV
        wnd.GetBuffer(wnd.GetCurrentBufferIndex())->SetResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET, list.ResourceBarrierPeekAndPush());
        // Clear RT
        list.ClearRenderTarget(wnd.GetRtvCpuHandle(wnd.GetCurrentBufferIndex()));

        // TODO: Composition
        state.Bind(list);
        rConf.Bind(list);

        // Set Resource to RTV
        wnd.GetBuffer(wnd.GetCurrentBufferIndex())->SetResourceState(D3D12_RESOURCE_STATE_PRESENT, list.ResourceBarrierPeekAndPush());
        
        // === Execute and Present ===
        list.Close();
        queue.Wait(queue.Execute(list));
        list.Reset();
        wnd.Present();
    }

    // Flush Queue
    queue.Flush(wnd.GetBufferCount());
    return 0;
}

INT WINAPI wWinMain(HINSTANCE _In_ hInstance, HINSTANCE _In_opt_ hInstPrev, PWSTR _In_ cmdArgs, INT _In_ cmdShow) {
    try {
        // Invoke safe
        return wWinMain_safe(hInstance, cmdArgs, cmdShow);
    }
    // Catch lib
    catch (Foundation::HRException& ex) {
        ex.show();
    }
    // Catch std
    catch (std::exception& ex) {
        MessageBoxA(NULL, ex.what(), "STD Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    // Catch unknown
    catch (...) {
        MessageBoxA(NULL, "Unknown exception", "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    // Return zero
    return 0;
}
