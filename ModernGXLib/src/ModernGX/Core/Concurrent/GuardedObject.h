#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Concurrent/ILock.h>
#include <ModernGX/Core/Concurrent/LockGuard.h>

#include <type_traits>
#include <exception>

namespace MGX::Core::Concurrent
{
    // Proxy object to access T
    template<typename T>
    class GuardedObjectProxy
    {
        public:
            // Construct
            GuardedObjectProxy() = default;
            GuardedObjectProxy(const GuardedObjectProxy&) = delete;
            
            // Prametrized construct
            GuardedObjectProxy(T* ptrObject, ILock* ptrLock) :
                m_lockGuard(ptrLock),
                m_ptrObject(ptrObject)
            {}

            // Move Construct
            GuardedObjectProxy(GuardedObjectProxy&& other) : 
                m_lockGuard(std::move(other.m_lockGuard)) 
            {
                m_ptrObject = other.m_ptrObject;
                other.m_ptrObject = nullptr;
            }

            // Delete assign operator 
            GuardedObjectProxy& operator=(const GuardedObjectProxy&) = delete;

            // Get reference to T
            T& Get() 
            {
                #ifdef _DEBUG
                // Check object pointer
                if (!m_ptrObject)
                {
                    throw std::exception("Get functioned called on invalid object guard");
                }
                #endif            

                // Return ref
                *m_ptrObject;
            }

            // Get pointer to T
            T* Ptr() noexcept
            {
                return m_ptrObject;
            }

            // Close proxy manualy
            void Close() noexcept
            {
                if (m_lockGuard)
                {
                    m_lockGuard.Close();
                    m_ptrObject = nullptr;
                }
            }

            // Auto convert to T*
            operator T* () noexcept
            {
                return m_ptrObject;
            }

            // Pointer acces operator
            T* operator->() noexcept 
            {
                return m_ptrObject;
            }

            // Boolean check
            operator bool() const noexcept
            {
                return m_ptrObject != nullptr;
            }

        private:
            // Pointer to proxy object
            T* m_ptrObject = nullptr;

            // The guard that locks the object
            LockGuard m_lockGuard;
    };

    // Core guarded object
    template<typename T, typename LT, typename = std::enable_if_t<std::is_base_of_v<ILock, LT>>>
    class GuardedObject 
    {
        public:
            // Default Construct
            GuardedObject() = default;
            GuardedObject(const GuardedObject&) = delete;
            GuardedObject(const T& tRef) : m_object(tRef) {}
            GuardedObject(const T&& tMov) : m_object(std::move(tMov)) {}

            // Special construct
            template<typename... ATs>
            GuardedObject(ATs... args) :
                m_object(T(args...))
            {}

            // Delete assign operator
            GuardedObject& operator=(const GuardedObject&) = delete;
            // Support move
            GuardedObject& operator=(GuardedObject&& other) = default;

            // Get Read only ref
            const T& Read() const noexcept
            {
                return m_object;
            }

            // Get Read only ptr
            const T* ReadPtr() const noexcept
            {
                return &m_object;
            }

            // Get read write access
            GuardedObjectProxy<T> GetRWProxy() noexcept
            {
                return GuardedObjectProxy<T>(&m_object, &m_lock);
            }

            // Conversion to const pointer
            explicit operator const T*() const noexcept
            {
                return &m_object;
            }

            // Pointer operator
            const T* operator->() const noexcept
            {
                return &m_object;
            }

        private:
            // Guarding lock
            LT m_lock = LT();

            // Object
            T m_object;
    };
}
