#pragma once

#include <ModernGX.h>

namespace MGX::Util
{
    // DXGI Format info
    class DXGIFormat
    {
        public:
            // No oop :/
            DXGIFormat() = delete;
            DXGIFormat(const DXGIFormat&) = delete;

            // Retrive bitpp / bytepp
            static unsigned int GetBpp(DXGI_FORMAT format) noexcept;
            inline static unsigned int GetBytedPerPixel(DXGI_FORMAT format) noexcept
            {
                return (GetBpp(format) + 7) / 8;
            }
    };
}
