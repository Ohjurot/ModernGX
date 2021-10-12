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

#include <ModernGX/Core/GPUTypes/GTypeBuffer.h>

// Windows enable visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace MGX;

struct Vertex
{
    float x, y, z, w;
};

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
    float buffer[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
    Core::GPU::PipelineState state(device, L"Test.xml");
    Core::GPU::RootConfiguration rConf(state.GetType(),
        Core::GPU::RootConfigurationEntry::MakeRootConstant(4, buffer)
    );

    Core::GType::Buffer vertexBuffer(device, Core::GPU::HeapUsage::Default, 64 * sizeof(Vertex));
    vertexBuffer.name(L"Vertex Buffer");
    Core::GType::Buffer vertexBufferUpl(device, Core::GPU::HeapUsage::Upload, 64 * sizeof(Vertex));
    vertexBufferUpl.name(L"Vertex Upload Buffer");

    Vertex vtxs[] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
    };

    auto mapped = vertexBufferUpl.Map();
    mapped.CopyToT<Vertex>(vtxs, __crt_countof(vtxs));
    mapped.Unmap();

    vertexBuffer.SetResourceState(D3D12_RESOURCE_STATE_COPY_DEST, list.ResourceBarrierPeekAndPush());
    vertexBufferUpl.SetResourceState(D3D12_RESOURCE_STATE_COPY_SOURCE, list.ResourceBarrierPeekAndPush());
    list.ResourceBarrierFlush();

    list->CopyBufferRegion(vertexBuffer, 0, vertexBufferUpl, 0, vertexBuffer.Size());
    vertexBuffer.SetResourceState(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, list.ResourceBarrierPeekAndPush());
    list.Close();
    queue.Wait(queue.Execute(list));
    list.Reset();

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

        auto handle = wnd.GetRtvCpuHandle(wnd.GetCurrentBufferIndex());
        list->OMSetRenderTargets(1, &handle, false, nullptr);

        RECT sicRect = { 0, 0, wnd.GetWidth(), wnd.GetHeight() };
        list->RSSetScissorRects(1, &sicRect);

        D3D12_VIEWPORT vp = {0, 0, wnd.GetWidth(), wnd.GetHeight(), 0.0f, 1.0f};
        list->RSSetViewports(1, &vp);

        D3D12_VERTEX_BUFFER_VIEW vbv;
        vbv.SizeInBytes = sizeof(Vertex) * __crt_countof(vtxs);
        vbv.StrideInBytes = sizeof(Vertex);
        vbv.BufferLocation = vertexBuffer[0];
        list->IASetVertexBuffers(0, 1, &vbv);
        list->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        
        list->DrawInstanced(4, 1, 0, 0);

        // Set Resource to Present
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
