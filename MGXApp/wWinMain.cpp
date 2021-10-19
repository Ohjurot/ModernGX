/*
* TODO: - Not much error informations (FIX NOW!)
* 
* LIMITS: - No UAV
*         - TextureCopy from/to buffer can only copy the full texture
*         - SRV for texture do not work for msaa
*         - No Multithread guards implemented (locking exists but is not used)
*         - WIC Image IO is just very basic (need way more attention)
*         - Upload buffer can only operate with DIRECT command list (not COPY)
*/
#include <ModernGX.h>
#include <ModernGX/Util/Memory.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GFXWindow.h>
#include <ModernGX/Core/WindowEventListener.h>
#include <ModernGX/Core/GPU/GPUDevice.h>
#include <ModernGX/Core/GPU/GPUQueue.h>
#include <ModernGX/Core/GPU/GPUCommandList.h>

#include <ModernGX/Core/GPU/GPUHeap.h>
#include <ModernGX/Core/GPU/GPUResource.h>

#include <ModernGX/Core/GPU/GPUPipelineState.h>
#include <ModernGX/Core/GPU/GPURootConfiguration.h>

#include <ModernGX/Core/GPUTypes/GTypeBuffer.h>
#include <ModernGX/Core/GPUTypes/GTypeTexture.h>
#include <ModernGX/Helpers/GPUUploadStack.h>

#include <ModernGX/Coding/TextureCoding.h>

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
    
    // Device1
    Core::GPU::Device device; device.name(L"Main device");

    // Commands
    Core::GPU::CommandQueue queue(device);
    Core::GPU::CommandList list(device);

    // RTV Heap
    Core::GPU::DescriptorHeap rtvDescHeap(device, Core::GPU::DescriptorHeapUsage::RenderTargetView, 16); 
    Core::GPU::DescriptorHeap srvDescHeap(device, Core::GPU::DescriptorHeapUsage::ShaderResource, 16); 

    // TEST
    
    // Upload buffer
    Helpers::GPUUploadStack uploadBuffer(device, 1024 * 1024 * 64);
    uploadBuffer.Reset(&list);

    // Vertex buffer
    Core::GType::Buffer vertexBuffer(device, Core::GPU::HeapUsage::Default, 64 * sizeof(Vertex));
    vertexBuffer.name(L"Vertex Buffer");
    Vertex vtxs[] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
    };
    uploadBuffer.CopyBuffer(&vertexBuffer, vtxs, sizeof(Vertex) * _countof(vtxs));
    
    // VBV View
    D3D12_VERTEX_BUFFER_VIEW vbv;
    vertexBuffer.CreateVBV<Vertex>(&vbv, __crt_countof(vtxs));

    // Texture 
    Coding::TextureFile textureImg(L"./test.png");
    Core::GType::Texture tex(device, Core::GPU::HeapUsage::Default, textureImg.GetDxgiFormat(), textureImg.GetWidth(), textureImg.GetHeight(), 0);
    uploadBuffer.CopyTexture(&tex, &textureImg);
    
    // SRV Handle
    auto srvHandle = srvDescHeap.Allocate(1);
    tex.CreateSRV(device, srvHandle[0]);

    // State changes
    vertexBuffer.SetResourceState(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, list.ResourceBarrierPeekAndPush());
    tex.SetResourceState(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list.ResourceBarrierPeekAndPush());

    // State and Root
    float buffer[4] = { 0.1f, 0.05f, 0.85f, 1.0f };
    Core::GPU::PipelineState state(device, L"Test.xml");
    Core::GPU::RootConfiguration rConf(state.GetType(), 2,
        Core::GPU::RootConfigurationEntry::MakeRootConstant(4, buffer),
        Core::GPU::RootConfigurationEntry::MakeDescriptorTable(srvHandle[0])
    );


    // No upload is required now
    uploadBuffer.Close();
    // END 

    // Create window
    Core::Window wnd(L"My Window", device, queue, rtvDescHeap.Allocate(3));
    D3D12_VIEWPORT windowViewport = wnd.GetViewport();
    RECT windowRect = wnd.GetScissorRect();

    // Prcoess window events
    while (wnd.ProcessWindowEvents()) {
        // Window resizing
        if (wnd.NeedsResizing()) {
            queue.Flush(wnd.GetBufferCount());
            wnd.ResizeNow(device);
            windowViewport = wnd.GetViewport();
            windowRect = wnd.GetScissorRect();
        }

        // Prepare buffer for RT
        wnd.GetCurrentBuffer()->SetResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET, list.ResourceBarrierPeekAndPush());
        list.OMPrepareRtDsViews(wnd.GetCurrentRtvCpuHandle());

        // Bind pipeline state
        list.SetHeaps(srvDescHeap);
        state.Bind(list);
        rConf.Bind(list);

        // Bind rasterizer
        list.RSSetViewportAndRect(&windowViewport, &windowRect);

        // Bind vertex buffer
        list.IASetBuffer(&vbv, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        
        // Draw
        list.Draw(4);

        // Prepare buffer for presenting
        wnd.GetCurrentBuffer()->SetResourceState(D3D12_RESOURCE_STATE_PRESENT, list.ResourceBarrierPeekAndPush());
        
        // Execute 
        list.Close();
        queue.Wait(queue.Execute(list));
        list.Reset();

        // Present frame
        wnd.Present(true);
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
