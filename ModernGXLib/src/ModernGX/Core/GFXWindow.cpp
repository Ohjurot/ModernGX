#include "ModernGX.h"
#include "GFXWindow.h"

EasyHWND::WindowClass MGX::Core::Window::s_wndCls(L"MGX_WND_CLS", CS_OWNDC);

MGX::Core::Window::Window(LPCWSTR title, ID3D12Device* ptrDevice, ID3D12CommandQueue* ptrCommandQueue, GPU::DescriptorRange rtvRange, bool borderless, bool trippleBuffering) :
    EasyHWND::Window(s_wndCls, title, 0, 0, 1920, 1080, borderless ? WS_POPUP : WS_OVERLAPPEDWINDOW),
    m_bufferCount(trippleBuffering ? 3 : 2),
    m_rtvRange(rtvRange)
{
    // Validate range
    if (m_rtvRange.GetSize() < 3)
    {
        throw std::exception("The MGX::Core::Window requires a descriptor range of minimum 3 descriptors");
    }

    // Get cursor screen
    POINT cursorPos = { 0,0 };
    GetCursorPos(&cursorPos);

    // Get monitor for cursor
    HMONITOR monitor = MonitorFromPoint(cursorPos, MONITOR_DEFAULTTONEAREST);

    // Window target rectg
    RECT windowRect = { 0, 0, 1920, 1080 };
    
    // Get monitor info
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfoW(monitor, &info)) 
    {
        windowRect = borderless ? info.rcMonitor : info.rcWork;
    }

    // Adjust rect if required
    if (!borderless) 
    {
        // Half size
        m_width = (windowRect.right - windowRect.left) / 2;
        m_height = (windowRect.bottom - windowRect.top) / 2;
        windowRect.top += m_height / 2;
        windowRect.bottom -= m_height / 2;
        windowRect.left += m_width / 2;
        windowRect.right -= m_width / 2;

        // Adjust rect
        AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, NULL);
    }
    else 
    {
        // Set sizes
        m_width = (windowRect.right - windowRect.left);
        m_height = (windowRect.bottom - windowRect.top);
    }

    // Set window bounds
    this->setWindowPosition(windowRect.left, windowRect.top);
    this->setWindowSize(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

    // Show window
    this->setWindowVisibility(true);

    // Maximize window
    ShowWindow(this->operator HWND(), SW_MAXIMIZE);

    // Get DXGI Factory
    ComPointer<IDXGIFactory2> ptrFactory;
    MGX_EVALUATE_HRESULT("CreateDXGIFactory2(...)", CreateDXGIFactory2(NULL, IID_PPV_ARGS(&ptrFactory)));

    // Describe swap chain
    DXGI_SWAP_CHAIN_DESC1 sd = {};
    sd.Width = m_width;
    sd.Height = m_height;
    sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.Stereo = FALSE;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferCount = GetBufferCount();
    sd.Scaling = DXGI_SCALING_STRETCH;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Describe fullscreen
    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd = {};
    fd.Windowed = TRUE;
    fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    fd.Scaling = DXGI_MODE_SCALING_STRETCHED;
    fd.RefreshRate.Numerator = 60;
    fd.RefreshRate.Denominator = 1;

    // Create swap chain
    ComPointer<IDXGISwapChain1> ptrSwapTemp;
    MGX_EVALUATE_HRESULT("IDXGIFactory2->CreateSwapChainForHwnd(...)", ptrFactory->CreateSwapChainForHwnd(ptrCommandQueue, this->operator HWND(), &sd, &fd, NULL, &ptrSwapTemp));

    // Query for SC3
    if (!ptrSwapTemp.queryInterface(m_ptrSwapChain))
    {
        throw std::exception("Your system does not support IDXGISwapChain3");
    }

    // Get buffers
    __getBuffers(ptrDevice);
}

MGX::Core::Window::~Window() 
{
    // Release buffers bevore swap chain
    __releaseBuffers();
    m_ptrSwapChain.release();
}

bool MGX::Core::Window::ProcessWindowEvents() noexcept 
{
    // Message loop
    MSG msg;
    while (PeekMessage(&msg, this->operator HWND(), 0, 0, PM_REMOVE)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Check for window events
    return !this->checkWindowCloseFlag(false);
}

void MGX::Core::Window::AddWindowEventListener(WindowEventListener* ptrListener) noexcept
{
    if (m_ptrFirstEventListener)
    {
        m_ptrFirstEventListener = m_ptrFirstEventListener->AddWindowEventListener(ptrListener);
    }
    else
    {
        m_ptrFirstEventListener = ptrListener;
    }
}

void MGX::Core::Window::RemoveWindowEventListener(WindowEventListener* ptrListener) noexcept
{
    if (m_ptrFirstEventListener)
    {
        m_ptrFirstEventListener = m_ptrFirstEventListener->RemoveWindowEventListener(ptrListener);
    }
}

bool MGX::Core::Window::NeedsResizing() noexcept 
{
    return m_needsResize;
}

void MGX::Core::Window::ResizeNow(ID3D12Device* ptrDevice) noexcept
{
    // Drop buffer
    __releaseBuffers();

    // Get client rect
    RECT cr;
    if (GetClientRect(this->operator HWND(), &cr)) 
    {
        // Update dimentsions
        m_width = cr.right - cr.left;
        m_height = cr.bottom - cr.top;

        // Resize swap chain
        m_ptrSwapChain->ResizeBuffers(GetBufferCount(), m_width, m_height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
        m_bufferIndex = m_ptrSwapChain->GetCurrentBackBufferIndex();

        // Resizing done
        m_needsResize = false;
    }

    // Get buffers
    __getBuffers(ptrDevice);
}

void MGX::Core::Window::Present(bool vsync) noexcept 
{
    m_ptrSwapChain->Present(vsync ? 1 : 0, NULL);
    m_bufferIndex = m_ptrSwapChain->GetCurrentBackBufferIndex();
}

unsigned int MGX::Core::Window::GetCurrentBufferIndex() noexcept 
{
    return m_bufferIndex;
}

MGX::Core::GPU::Resource* MGX::Core::Window::GetBuffer(unsigned int idx) noexcept 
{
    return idx < GetBufferCount() ? &m_ptrBuffers[idx] : nullptr;
}

MGX::Core::GPU::Resource* MGX::Core::Window::GetCurrentBuffer() noexcept
{
    return &m_ptrBuffers[m_bufferIndex];
}

D3D12_CPU_DESCRIPTOR_HANDLE MGX::Core::Window::GetRtvCpuHandle(unsigned int idx) noexcept
{
    return idx < GetBufferCount() ? m_rtvRange[idx].cpu : D3D12_CPU_DESCRIPTOR_HANDLE({ 0 });
}

D3D12_CPU_DESCRIPTOR_HANDLE MGX::Core::Window::GetCurrentRtvCpuHandle() noexcept
{
    return m_rtvRange[m_bufferIndex].cpu;
}

D3D12_VIEWPORT MGX::Core::Window::GetViewport(float minDepth, float maxDepth, UINT left, UINT top, UINT width, UINT height) const noexcept
{
    // Safly build viewport
    D3D12_VIEWPORT vp;
    vp.TopLeftX = std::min<FLOAT>((FLOAT)left, (FLOAT)m_width);
    vp.TopLeftY = std::min<FLOAT>((FLOAT)top, (FLOAT)m_width);
    vp.Width = std::min<FLOAT>((FLOAT)width, (FLOAT)m_width - vp.TopLeftX);
    vp.Height = std::min<FLOAT>((FLOAT)height, (FLOAT)m_height - vp.TopLeftY);
    vp.MinDepth = minDepth;
    vp.MaxDepth = maxDepth;

    // Return viewport
    return vp;
}

RECT MGX::Core::Window::GetScissorRect(UINT top, UINT left, UINT width, UINT height) const noexcept
{
    // Safly build scissor rect
    RECT sicRect;
    sicRect.top = std::min<LONG>((LONG)top, (LONG)m_height);
    sicRect.left = std::min<LONG>((LONG)left, (LONG)m_width);
    sicRect.right = sicRect.left + (LONG)std::min<UINT>(width, m_width - sicRect.left);
    sicRect.bottom = sicRect.top + (LONG)std::min<UINT>(height, m_height - sicRect.top);

    // Return rect
    return sicRect;
}

void MGX::Core::Window::__getBuffers(ID3D12Device* ptrDevice)
{
    // Get each buffer
    for (unsigned int i = 0; i < GetBufferCount(); i++) 
    {
        // Get buffer
        ComPointer<ID3D12Resource> ptrResource = nullptr;
        m_ptrSwapChain->GetBuffer(i, IID_PPV_ARGS(&ptrResource));

        // Store buffer
        m_ptrBuffers[i] = GPU::Resource(ptrResource, D3D12_RESOURCE_STATE_PRESENT);

        // Create descriptor
        ptrDevice->CreateRenderTargetView(ptrResource, NULL, m_rtvRange[i].cpu);

        // Naming
        #ifndef MGX_DISABLE_INTERNAL_D3D_NAMEING
        // Get number as string
        WCHAR count[2];
        *count = 0x00;
        _ultow_s<2>(i, count, 10);
        
        // Get window name
        WCHAR wndTitle[512];
        *wndTitle = 0x00;
        GetWindowText(this->operator HWND(), wndTitle, 512);

        // Combine string
        WCHAR name[512];
        *name = 0x00;
        wcscpy_s<512>(name, L"Back Buffer #");
        wcscat_s<512>(name, count);
        wcscat_s<512>(name, L" (For window: \"");
        wcscat_s<512>(name, wndTitle);
        wcscat_s<512>(name, L"\")");

        // Name object
        m_ptrBuffers[i].name(name);
        #endif
    }
}

void MGX::Core::Window::__releaseBuffers() noexcept 
{
    // Release buffers
    for (unsigned int i = 0; i < GetBufferCount(); i++) 
    {
        m_ptrBuffers[i].~Resource();
    }
}

bool MGX::Core::Window::handleWindowMessage(LRESULT* ptrLRESULT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    // Switch on messages
    switch (msg) 
    {
        // Size message
        case WM_SIZE: 
        {
            if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED) 
            {
                // Get client rect
                RECT cr;
                if (GetClientRect(this->operator HWND(), &cr)) 
                {
                    // Check resize
                    if (cr.right - cr.left != m_width || cr.bottom - cr.top != m_height) 
                    {
                        m_needsResize = true;
                    }
                }
            }
            break;
        }
    }

    // Wrapp into window event
    WindowEvent e;
    e.wnd = hwnd;
    e.msg = msg;
    e.wParam = wParam;
    e.lParam = lParam;

    // Check if any listeners handles the event
    if (m_ptrFirstEventListener && m_ptrFirstEventListener->HandleEvent(&e))
    {
        *ptrLRESULT = e.eventResult;
        return true;
    }
    else
    {
        // Send to parent handler
        return EasyHWND::Window::handleWindowMessage(ptrLRESULT, hwnd, msg, wParam, lParam);
    }
}
