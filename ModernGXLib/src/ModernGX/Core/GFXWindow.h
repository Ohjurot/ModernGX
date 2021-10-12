#pragma once

#include <ModernGX.h>
#include <ModernGX/Util/EasyHWND.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GPU/GPUResource.h>
#include <ModernGX/Core/GPU/GPUDescriptorHeap.h>

#include <exception>

namespace MGX::Core {
    // Window class
    class Window : private EasyHWND::Window 
    {
        public:
            // Construct
            Window() = delete;
            Window(const Window&) = delete;
            Window(LPCWSTR title, ID3D12Device* ptrDevice, ID3D12CommandQueue* ptrCommandQueue, GPU::DescriptorRange rtvRange, bool borderless = false, bool trippleBuffering = false);

            // Destructor
            ~Window();

            // Window functions
            bool ProcessWindowEvents() noexcept;

            // Swap chain sizing
            bool NeedsResizing() noexcept;
            void ResizeNow(ID3D12Device* ptrDevice) noexcept;

            // Present window
            void Present(bool vsync = false) noexcept;

            // Buffers
            unsigned int GetCurrentBufferIndex() noexcept;
            GPU::Resource* GetBuffer(unsigned int idx) noexcept;
            inline unsigned int GetBufferCount() const noexcept 
            {
                return m_bufferCount;
            }

            // Get width
            inline UINT GetWidth() const noexcept
            {
                return m_width;
            }

            // Get height
            inline UINT GetHeight() const noexcept
            {
                return m_height;
            }

            // Rtv Handle
            D3D12_CPU_DESCRIPTOR_HANDLE GetRtvCpuHandle(unsigned int idx) noexcept;

            // Vieport function
            D3D12_VIEWPORT GetViewport(float minDepth = 1.0f, float maxDepth = 0.0f, UINT left = 0, UINT top = 0, UINT width = UINT_MAX, UINT height = UINT_MAX) const noexcept;
            RECT GetScissorRect(UINT top = 0, UINT left = 0, UINT width = UINT_MAX, UINT height = UINT_MAX) const noexcept;

        private:
            // Buffer release and construct
            void __getBuffers(ID3D12Device* ptrDevice);
            void __releaseBuffers() noexcept;

        protected:
            // Window message handler
            virtual bool handleWindowMessage(LRESULT* ptrLRESULT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

        private:
            // Window class
            static EasyHWND::WindowClass s_wndCls;

            // Swap chain pointer
            ComPointer<IDXGISwapChain3> m_ptrSwapChain;

            // Buffers
            GPU::Resource m_ptrBuffers[3];
            unsigned int m_bufferIndex = 0;
            const unsigned int m_bufferCount;

            // RTV Descriptors (Size >= 3)
            GPU::DescriptorRange m_rtvRange;

            // Size
            UINT m_width = 0;
            UINT m_height = 0;
            bool m_needsResize = false;
    };
}