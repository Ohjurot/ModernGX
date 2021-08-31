#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Util/EasyHWND.h>

namespace MGX {
    namespace Core {
        // Window class
        class Window : private EasyHWND::Window {
            public:
                // Construct
                Window() = delete;
                Window(const Window&) = delete;
                Window(LPCWSTR title, ID3D12CommandQueue* ptrCommandQueue, bool borderless = false, bool trippleBuffering = false);

                // Destructor
                ~Window();

                // Window functions
                bool ProcessWindowEvents() noexcept;

                // Swap chain sizing
                bool NeedsResizing() noexcept;
                void ResizeNow() noexcept;

                // Present window
                void Present(bool vsync = false) noexcept;

                // Buffers
                unsigned int GetCurrentBufferIndex() noexcept;
                ID3D12Resource* GetBuffer(unsigned int idx) noexcept;
                inline unsigned int GetBufferCount() const noexcept {
                    return m_bufferCount;
                }

            private:
                // Buffer release and construct
                void __getBuffers();
                void __releaseBuffers() noexcept;

            protected:
                // Window message handler
                virtual bool handleWindowMessage(LRESULT* ptrLRESULT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

            private:
                // Window class
                static EasyHWND::WindowClass s_wndCls;

                // Swap chain pointer
                ComPointer<IDXGISwapChain1> m_ptrSwapChain;

                // Buffers
                ComPointer<ID3D12Resource> m_ptrBuffers[3];
                unsigned int m_bufferIndex = 0;
                const unsigned int m_bufferCount;

                // Size
                UINT m_width = 0;
                UINT m_height = 0;
                bool m_needsResize = false;
        };
    }
}