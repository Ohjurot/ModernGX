/*
*
* TODO:   - Upload Buffers
* 
* LIMITS: - No UAV
*         - Not much error informations
*         - TextureCopy from/to buffer can only copy the full texture
*         - SRV for texture do not work for msaa
*         - No Multithread guards implemented (locking exists but is not used)
*         - WIC Image IO is just very basic (need way more attention)
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
    
    // Vertex buffer (incl. upload buffer)
    Core::GType::Buffer vertexBuffer(device, Core::GPU::HeapUsage::Default, 64 * sizeof(Vertex));
    vertexBuffer.name(L"Vertex Buffer");
    Core::GType::Buffer vertexBufferUpl(device, Core::GPU::HeapUsage::Upload, 32 * 1024 * 1024);
    vertexBufferUpl.name(L"Vertex Upload Buffer");

    // CPU Load buffer
    Vertex vtxs[] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
    };

    Coding::TextureFile textureImg(L"./test.png");
    auto imgStride = textureImg.GetChannelCount() * textureImg.GetWidth();
    auto imgSize = imgStride * textureImg.GetHeight();
    void* img = malloc(imgSize);
    auto res = textureImg.ReadPixels(0, 0, textureImg.GetWidth(), textureImg.GetHeight(), img, imgSize, imgStride);

    // View
    D3D12_VERTEX_BUFFER_VIEW vbv;
    vertexBuffer.CreateVBV<Vertex>(&vbv, __crt_countof(vtxs));

    // Copy to cpu <--> cpu
    auto mapped = vertexBufferUpl.Map();
    mapped.TCopyFrom<Vertex>(vtxs, __crt_countof(vtxs));
    mapped.CopyFrom(img, imgSize, 1024 * 64);
    mapped.Unmap();

    // Copy GPU
    vertexBufferUpl.CopyTo(&list, &vertexBuffer, sizeof(Vertex) * __crt_countof(vtxs));
    vertexBuffer.SetResourceState(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, list.ResourceBarrierPeekAndPush());


    // Texture 
    Core::GType::Texture tex(device, Core::GPU::HeapUsage::Default, textureImg.GetDxgiFormat(), textureImg.GetWidth(), textureImg.GetHeight(), 0);
    tex.CopyFromBuffer(&list, &vertexBufferUpl, 1024 * 64);
    tex.SetResourceState(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list.ResourceBarrierPeekAndPush());
    auto srvHandle = srvDescHeap.Allocate(1);
    tex.CreateSRV(device, srvHandle[0]);

    float buffer[4] = { 0.1f, 0.05f, 0.85f, 1.0f };
    Core::GPU::PipelineState state(device, L"Test.xml");
    Core::GPU::RootConfiguration rConf(state.GetType(), 2,
        Core::GPU::RootConfigurationEntry::MakeRootConstant(4, buffer),
        Core::GPU::RootConfigurationEntry::MakeDescriptorTable(srvHandle[0])
    );

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
