#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/Allocator/IAllocator.h>

namespace MGX 
{
    // Killbyte
    constexpr size_t MemKiB(size_t kib) noexcept
    {
        return kib * 1024ULL;
    }

    // Megabyte
    constexpr size_t MemMiB(size_t mib) noexcept
    {
        return mib * 1024ULL * 1024ULL;
    }

    // Gigabyte
    constexpr size_t MemGiB(size_t gib) noexcept
    {
        return gib * 1024ULL * 1024ULL * 1024ULL;
    }

    // Terrabyte
    constexpr size_t MemTiB(size_t tib) noexcept
    {
        return tib * 1024ULL * 1024ULL * 1024ULL * 1024ULL;
    }

    // Try (throw) memory allocator
    template<typename T>
    inline T* TryAllocate(Core::IAllocator& ptrAllocator = *Core::StdAllocator::Instance(), size_t count = 1)
    {
        // Allocate
        T* pt = ptrAllocator.TAllocate<T>(count);
        
        // Exception
        if (!pt)
        {
            throw std::exception("Memory allocation failed");
        }

        return pt;
    }

    // Allocate and check (return value)
    template<typename T>
    inline bool AllocateCheck(T** ppT, Core::IAllocator& ptrAllocator = *Core::StdAllocator::Instance(), size_t count = 1)
    {
        // Allocate
        *ppT = ptrAllocator.TAllocate<T>(count);

        // Return 
        return *ppT != nullptr;
    }
}
