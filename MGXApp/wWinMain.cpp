#include <ModernGX.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GFXWindow.h>
#include <ModernGX/Core/GPU/GPUDevice.h>
#include <ModernGX/Core/GPU/GPUQueue.h>

using namespace MGX;

INT wWinMain_safe(HINSTANCE hInstance, PWSTR cmdArgs, INT cmdShow) {
    // Enable debug layer
    #ifdef _DEBUG
    ComPointer<ID3D12Debug> dbg;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dbg)))) {
        dbg->EnableDebugLayer();
    }
    #endif
    
    // Device
    Core::GPU::Device device;
    Core::GPU::CommandQueue queue(device);
    
    // Create window
    Core::Window wnd(L"My Window", queue);

    // Prcoess window events
    while (wnd.ProcessWindowEvents()) {
        // Window resizing
        if (wnd.NeedsResizing()) {
            queue.Flush(wnd.GetBufferCount());
            wnd.ResizeNow();
        }

        // TODO: Render loop
        queue.Flush(1);
        
        // Present frame
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
