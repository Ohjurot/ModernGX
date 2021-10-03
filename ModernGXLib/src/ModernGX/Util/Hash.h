#pragma once

#include <ModernGX.h>

namespace MGX
{
    // Combined Hash value
    typedef struct __H128
    {
        UINT64 lp, hp;
    } H128;
}

namespace MGX::Util
{
    // Hash values
    UINT32 Hash32Bit(const void* in, size_t len, UINT32 seed = 0) noexcept;
    UINT64 Hash64Bit(const void* in, size_t len, UINT64 seed = 0) noexcept;
    H128 Hash128Bit(const void* in, size_t len, UINT64 seed = 0) noexcept;
}

// Compare hash
inline bool operator==(const MGX::H128& a, const MGX::H128& b) noexcept
{
    return a.lp == b.lp && a.hp == b.hp;
}
