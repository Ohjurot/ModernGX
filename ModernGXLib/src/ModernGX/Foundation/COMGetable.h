#pragma once
#include <ModernGX.h>

namespace MGX {
    namespace Foundation {
        // Easy public COM encapsulator
        template<typename CT>
        class COMGetable {
            public:
                // Convert to pointer
                operator CT* () noexcept {
                    return m_ptrBase;
                }

                // Pointer operator
                CT* operator->() noexcept {
                    return m_ptrBase;
                }

                // Bool operator (virutal)
                virtual operator bool() noexcept {
                    return m_ptrBase != nullptr;
                }

                // Get pointer
                CT* Ptr() {
                    return m_ptrBase;
                }

                // Ge it Ass
                template<typename T>
                ComPointer<T> As() {
                    ComPointer<T> ptr;
                    if (m_ptrBase) {
                        m_ptrBase.queryInterface(ptr);
                    }
                    return ptr;
                }

                // Naming the object
                virtual HRESULT name(LPCWSTR name) {
                    return m_ptrBase.name<CT>(name);
                }

            protected:
                // Exposed base pointer (public)
                ComPointer<CT> m_ptrBase;
        };
    }
}
