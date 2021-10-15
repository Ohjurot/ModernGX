#include "ModernGX.h"
#include "WindowEventListener.h"

MGX::Core::WindowEventListener::WindowEventListener(INT priority) :
    m_priority(priority)
{ }

MGX::Core::WindowEventListener* MGX::Core::WindowEventListener::AddWindowEventListener(WindowEventListener* ptrListener) noexcept
{
    // Check if insert before is required
    if (ptrListener->m_priority >= m_priority)
    {
        // caller --> ptrListener --> this
        ptrListener->m_ptrNextListener = this;
        return ptrListener;
    }
    // Check if next listener can handle call
    else if (m_ptrNextListener)
    {
        // caller --> this --> m_ptrNextListener/ptrListener
        m_ptrNextListener = m_ptrNextListener->AddWindowEventListener(ptrListener);
    }
    else
    {
        // caller --> this --> ptrListener
        m_ptrNextListener = ptrListener;
    }

    // This is next
    return this;
}

MGX::Core::WindowEventListener* MGX::Core::WindowEventListener::RemoveWindowEventListener(WindowEventListener* ptrListener) noexcept
{
    if (ptrListener == this)
    {
        // Ommit this
        return m_ptrNextListener;
    }
    else 
    {
        // Update next and keep this
        m_ptrNextListener = m_ptrNextListener->RemoveWindowEventListener(ptrListener);
        return this;
    }
}

bool MGX::Core::WindowEventListener::HandleEvent(WindowEvent* ptrEvent)
{
    // Call current this handler
    switch (OnWindowEvent(*ptrEvent))
    {
        // Event handled --> direct return
        case WindowEventResult::Handled:
            ptrEvent->handled = true;
            return true;
            break;
        // Event handled but pass on! (Falltrough)
        case WindowEventResult::HandledPassOn:
            ptrEvent->handled = true;
            __fallthrough;
        // Event not handled (default) --> pass on
        case WindowEventResult::NotHandled:
            __fallthrough;
        default:
            return m_ptrNextListener ? m_ptrNextListener->HandleEvent(ptrEvent) : ptrEvent->handled;
    }    
}
