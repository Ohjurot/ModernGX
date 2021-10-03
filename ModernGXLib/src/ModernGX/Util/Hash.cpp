#include "ModernGX.h"
#include "Hash.h"

#include <xxhash/xxhash.h>

UINT32 MGX::Util::Hash32Bit(const void* in, size_t len, UINT32 seed) noexcept
{
    return XXH32(in, len, seed);
}

UINT64 MGX::Util::Hash64Bit(const void* in, size_t len, UINT64 seed) noexcept
{
    return XXH3_64bits_withSeed(in, len, seed);
}

MGX::H128 MGX::Util::Hash128Bit(const void* in, size_t len, UINT64 seed) noexcept
{
    auto hash = XXH3_128bits_withSeed(in, len, seed);
    return { hash.low64, hash.high64 };
}
