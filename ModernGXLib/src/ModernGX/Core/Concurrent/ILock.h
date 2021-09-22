#pragma once

#include <ModernGX.h>

namespace MGX::Core::Concurrent 
{
    // Inteface for locks
    class ILock
    {
        public:
            // Try to aquire the lock and return the result
            virtual bool TryAquire() noexcept = 0;
            // Release the lock
            virtual void Release() noexcept = 0;

            // Function that does nothing
            static inline void __noActionWait() noexcept {}

            // Aquire function
            template<typename FT = void()>
            inline void Aquire(FT func = &__noActionWait) noexcept 
            {
                while (!TryAquire())
                {
                    func();
                }
            }
    };
}
