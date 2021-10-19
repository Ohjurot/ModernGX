#pragma once

#include <ModernGX.h>
#include <ModernGX/Util/Hash.h>

#include <initializer_list>

namespace MGX::Helpers
{
    template<typename T, unsigned int C>
    class HashedBuffer
    {
        public:
            // Construct
            HashedBuffer() = default;
            HashedBuffer(const HashedBuffer&) = default;
            HashedBuffer(std::initializer_list<T> list)
            {
                for (unsigned int i = 0; i < C && i < list.size(); i++)
                {
                    m_buffer = list[i];
                }
            }
            HashedBuffer(T preset)
            {
                for (unsigned int i = 0; i < C; i++)
                    m_buffer[i] = preset;
            }
            HashedBuffer(const T& refPreset)
            {
                for (unsigned int i = 0; i < C; i++)
                    m_buffer[i] = refPreset;
            }

            // Raw access
            inline T* Ptr() noexcept
            {
                return m_buffer;
            }
            inline constexpr size_t Size() const noexcept
            {
                return sizeof(T) * C;
            }
            inline constexpr size_t Stride() const noexcept
            {
                return sizeof(T);
            }

            // Member access
            inline T& operator[](unsigned int idx)
            {
                return m_buffer[idx];
            }
            inline T& Get(unsigned int idx)
            {
                return m_buffer[idx];
            }

            // Hash functions
            inline UINT64 GetLastHash() const noexcept
            {
                return m_lastHash;
            }
            inline UINT64 GetCurrentHash() const noexcept
            {
                return Util::Hash64Bit(m_buffer, sizeof(T) * C);
            }

            // Update function
            void UpdateHash() noexcept
            {
                m_lastHash = GetCurrentHash();
            }

            // Check function
            bool HasChanges(bool updateHash = false)
            {
                UINT64 currentHash = GetCurrentHash();
                bool hasChanges = currentHash != m_lastHash;
                if (hasChanges && updateHash)
                    m_lastHash = currentHash;

                return hasChanges;
            }

        private:
            // Stacked buffer
            T m_buffer[C] = {};

            // Hash value
            UINT64 m_lastHash = 0;
    };
}
