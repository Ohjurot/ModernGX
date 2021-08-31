#include "ModernGX.h"
#include "GFXWindow.h"

EasyHWND::WindowClass MGX::Core::Window::s_wndCls(L"MGX_WND_CLS", CS_OWNDC);

MGX::Core::Window::Window(LPCWSTR title, ID3D12CommandQueue* ptrCommandQueue, bool borderless, bool trippleBuffering) :
    EasyHWND::Window(s_wndCls, title, 0, 0, 1920, 1080, borderless ? WS_POPUP : WS_OVERLAPPEDWINDOW),
    m_bufferCount(trippleBuffering ? 3 : 2)
{
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
    if (GetMonitorInfoW(monitor, &info)) {
        windowRect = borderless ? info.rcMonitor : info.rcWork;
    }

    // Adjust rect if required
    if (!borderless) {
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
    else {
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
    MGX_EVALUATE_HRESULT("IDXGIFactory2->CreateSwapChainForHwnd(...)", ptrFactory->CreateSwapChainForHwnd(ptrCommandQueue, this->operator HWND(), &sd, &fd, NULL, &m_ptrSwapChain));

    // Get buffers
    __getBuffers();
}

MGX::Core::Window::~Window() {
    // Release buffers bevore swap chain
    __releaseBuffers();
    m_ptrSwapChain.release();
}

bool MGX::Core::Window::ProcessWindowEvents() noexcept {
    // Message loop
    MSG msg;
    while (PeekMessage(&msg, this->operator HWND(), 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Check for window events
    return !this->checkWindowCloseFlag(false);
}

bool MGX::Core::Window::NeedsResizing() noexcept {
    return m_needsResize;
}

void MGX::Core::Window::ResizeNow() noexcept {
    // Drop buffer
    __releaseBuffers();

    // Get client rect
    RECT cr;
    if (GetClientRect(this->operator HWND(), &cr)) {
        // Update dimentsions
        m_width = cr.right - cr.left;
        m_height = cr.bottom - cr.top;

        // Resize swap chain
        m_ptrSwapChain->ResizeBuffers(GetBufferCount(), m_width, m_height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

        // Resizing done
        m_needsResize = false;
    }

    // Get buffers
    __getBuffers();
}

void MGX::Core::Window::Present(bool vsync) noexcept {
    m_ptrSwapChain->Present(vsync ? 1 : 0, NULL);
    m_bufferIndex = (m_bufferIndex + 1) % GetBufferCount();
}

unsigned int MGX::Core::Window::GetCurrentBufferIndex() noexcept {
    return m_bufferIndex;
}

ID3D12Resource* MGX::Core::Window::GetBuffer(unsigned int idx) noexcept {
    return idx < GetBufferCount() ? m_ptrBuffers[idx] : nullptr;
}

void MGX::Core::Window::__getBuffers() {
    // Get each buffer
    for (unsigned int i = 0; i < GetBufferCount(); i++) {
        // Get buffer
        m_ptrSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ptrBuffers[i]));

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

void MGX::Core::Window::__releaseBuffers() noexcept {
    // Release buffers
    for (unsigned int i = 0; i < GetBufferCount(); i++) {
        m_ptrBuffers[i].release();
    }
}

bool MGX::Core::Window::handleWindowMessage(LRESULT* ptrLRESULT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Switch on messages
    switch (msg) {
        // Size message
        case WM_SIZE: {
            if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED) {
                // Get client rect
                RECT cr;
                if (GetClientRect(this->operator HWND(), &cr)) {
                    // Check resize
                    if (cr.right - cr.left != m_width || cr.bottom - cr.top != m_height) {
                        m_needsResize = true;
                    }
                }
            }
            break;
        }
    }

    // Send to parent handler
    return EasyHWND::Window::handleWindowMessage(ptrLRESULT, hwnd, msg, wParam, lParam);
}