#pragma once

#include <ModernGX.h>

#include <memory>

namespace MGX::Core 
{
    // Memory allocator interface
    class IAllocator 
    {
        public:
            // Allocate memory
            virtual void* Allocate(size_t size) noexcept = 0;
            // Free memory
            virtual void Free(void* memory) noexcept = 0;

            // Fancy allocate
            template<typename T>
            inline T* TAllocate(size_t count) noexcept
            {
                return (T*)Allocate(sizeof(T) * count);
            }
    };

    // Monitored allocator
    class IMonitoredAllocator : public IAllocator
    {
        public:
            // Get total allocator size
            virtual UINT64 GetByteSize() const noexcept = 0;
            // Get total allocator usage
            virtual UINT64 GetByteUsage() const noexcept = 0;
            
            // Get normalized usage (0 ... 1)
            inline float GetUsage() const noexcept {
                return (float)GetByteUsage() / (float)GetByteSize();
            }
    };

    // Std allocator
    class StdAllocator : public IAllocator
    {
        public:
            // Implement interface
            void* Allocate(size_t size) noexcept override;
            void Free(void* memory) noexcept override;

            // Get instance
            static StdAllocator* Instance() noexcept;

        private:
            // Default instance
            static StdAllocator s_instance;
    };
}
