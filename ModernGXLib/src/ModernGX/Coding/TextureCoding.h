#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/HRException.h>

namespace MGX::Coding
{
    // WIC Wrapper
    class TextureFile
    {
        public:
            // Construct
            TextureFile() = default;
            TextureFile(const TextureFile&) = default;
            TextureFile(const wchar_t* fileName); // From filepath
            TextureFile(const void* fileContent, UINT fileLen); // From memory
            TextureFile(UINT width, UINT height, GUID fmt); // Empty WIC Format
            TextureFile(UINT width, UINT height, DXGI_FORMAT fmt); // Empty DXGI_FORMAT

            // Simple getters
            inline UINT GetWidth() const noexcept
            {
                return m_width;
            }
            inline UINT GetHeight() const noexcept
            {
                return m_height;
            }
            inline UINT GetBPP() const noexcept
            {
                return m_bpp;
            }
            inline UINT GetChannelCount() const noexcept
            {
                return m_channelCount;
            }
            inline GUID GetWicGuidFormat() const noexcept
            {
                return m_wicFormat;
            }
            inline DXGI_FORMAT GetDxgiFormat() const noexcept
            {
                return m_dxgiFormat;
            }

            // Read pixels from this class
            bool ReadPixels(UINT x, UINT y, UINT width, UINT height, void* dstBuffer, UINT dstSize, UINT dstStride, DXGI_FORMAT requestedFormat = DXGI_FORMAT_UNKNOWN);

        private:
            // Create factory
            void createWicFactory();
            // Retrive meta information of bitmap
            void getMetaInfo();

        private:
            // WIC Core objects
            ComPointer<IWICImagingFactory2> m_ptrWicFactory;
            ComPointer<IWICBitmapFrameDecode> m_ptrSrcDecoder;
            ComPointer<IWICBitmap> m_ptrSrcBitmap;

            // Meta information
            GUID m_wicFormat = {};
            DXGI_FORMAT m_dxgiFormat = DXGI_FORMAT_UNKNOWN;
            UINT m_width = 0, m_height = 0;
            UINT m_bpp = 0, m_channelCount = 0;

        private:
            // GUID <--> DXGI pairs
            struct GuidDXGI 
            {
                GUID guid;
                DXGI_FORMAT dxgi;
            };

            // Equivalent table
            static GuidDXGI s_equivalentFormats[];

            // Conversion functions
            static GUID ConvertDxgiFormatToGuid(DXGI_FORMAT fmt) noexcept;
            static DXGI_FORMAT ConvertGuidFormatToDxgi(GUID fmt) noexcept;
    };
}
