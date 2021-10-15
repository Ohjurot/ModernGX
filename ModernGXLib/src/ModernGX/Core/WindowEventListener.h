#pragma once

#include <ModernGX.h>

namespace MGX::Core
{
    // Fwd decl window
    class Window;

    // Result of window event
    enum class WindowEventResult : unsigned char
    {
        NotHandled = 0,
        HandledPassOn = 1,
        Handled = 2,
    };

    // Result of an window event
    struct WindowEvent
    {
        // Event input arguments
        HWND wnd = 0;
        UINT msg = 0;
        WPARAM wParam = 0;
        LPARAM lParam = 0;

        // Event output argument
        LRESULT eventResult = 0;
        bool handled = false;
    };

    // Window event listener base
    class WindowEventListener
    {
        public:
            // Construct
            WindowEventListener() = default;
            WindowEventListener(const WindowEventListener&) = delete;
            WindowEventListener(INT priority);

            // No copy
            WindowEventListener& operator=(const WindowEventListener&) = delete;

            // Virtual window event function
            virtual MGX::Core::WindowEventResult OnWindowEvent(MGX::Core::WindowEvent& e) = 0;

        protected:
            // Grant window
            friend class Window;

            // Modify window event chain
            WindowEventListener* AddWindowEventListener(WindowEventListener* ptrListener) noexcept;
            WindowEventListener* RemoveWindowEventListener(WindowEventListener* ptrListener) noexcept;

            // Event handlign routine
            bool HandleEvent(WindowEvent* ptrEvent);

        private:
            // Next listener in chain
            WindowEventListener* m_ptrNextListener = nullptr;
            // Listener priority
            INT m_priority = 0;
    };
}
