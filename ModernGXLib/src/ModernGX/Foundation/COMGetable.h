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

                // Ge it Ass
                template<typename T>
                ComPointer<T> As() {
                    ComPointer<T> ptr;
                    if (m_ptrBase) {
                        m_ptrBase.queryInterface(ptr);
                    }
                    return ptr;
                }

            protected:
                // Exposed base pointer (public)
                ComPointer<CT> m_ptrBase;
        };
    }
}
