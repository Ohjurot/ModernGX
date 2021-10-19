#include "ModernGX.h"
#include "TextureCoding.h"

MGX::Coding::TextureFile::GuidDXGI MGX::Coding::TextureFile::s_equivalentFormats[] =
{
    // Gray
    { GUID_WICPixelFormat8bppGray, DXGI_FORMAT_R8_UNORM },
    { GUID_WICPixelFormat16bppGray, DXGI_FORMAT_R16_UNORM },
    { GUID_WICPixelFormat16bppGrayHalf, DXGI_FORMAT_R16_FLOAT },
    { GUID_WICPixelFormat32bppGrayFloat, DXGI_FORMAT_R32_FLOAT },

    // RGB / BGR
    { GUID_WICPixelFormat32bppRGBA, DXGI_FORMAT_R8G8B8A8_UNORM },
    { GUID_WICPixelFormat32bppBGRA, DXGI_FORMAT_B8G8R8A8_UNORM },
    { GUID_WICPixelFormat64bppRGBA, DXGI_FORMAT_R16G16B16A16_UNORM },
    { GUID_WICPixelFormat64bppRGBAHalf, DXGI_FORMAT_R16G16B16A16_FLOAT },
    { GUID_WICPixelFormat128bppRGBAFloat, DXGI_FORMAT_R32G32B32A32_FLOAT },

    // Sentinel
    { 0, 0 },
};

MGX::Coding::TextureFile::TextureFile(const wchar_t* fileName)
{
    createWicFactory();

    // Open decoder
    ComPointer<IWICBitmapDecoder> ptrDecoder;
    MGX_EVALUATE_HRESULT("IWICImagingFactory2->CreateDecoderFromFilename(...)", m_ptrWicFactory->CreateDecoderFromFilename(fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &ptrDecoder));

    // Get first frame and format
    MGX_EVALUATE_HRESULT("IWICBitmapDecoder->GetFrame(...)", ptrDecoder->GetFrame(0, &m_ptrSrcDecoder));
    MGX_EVALUATE_HRESULT("IWICBitmapDecoder->GetPixelFormat(...)", m_ptrSrcDecoder->GetPixelFormat(&m_wicFormat));
    m_dxgiFormat = ConvertGuidFormatToDxgi(m_wicFormat);

    getMetaInfo();
}

MGX::Coding::TextureFile::TextureFile(const void* fileContent, UINT fileLen)
{
    createWicFactory();

    // Create Stream
    ComPointer<IStream> ptrStream = SHCreateMemStream((const BYTE*)fileContent, fileLen);
    MGX_EVALUATE_HRESULT("SHCreateMemStream(...)", ptrStream ? S_OK : E_FAIL);

    // Open Decoder
    ComPointer<IWICBitmapDecoder> ptrDecoder;
    MGX_EVALUATE_HRESULT("IWICImagingFactory2->CreateDecoderFromStream(...)", m_ptrWicFactory->CreateDecoderFromStream(ptrStream, NULL, WICDecodeMetadataCacheOnLoad, &ptrDecoder));

    // Get first frame and format
    MGX_EVALUATE_HRESULT("IWICBitmapDecoder->GetFrame(...)", ptrDecoder->GetFrame(0, &m_ptrSrcDecoder));
    MGX_EVALUATE_HRESULT("IWICBitmapDecoder->GetPixelFormat(...)", m_ptrSrcDecoder->GetPixelFormat(&m_wicFormat));
    m_dxgiFormat = ConvertGuidFormatToDxgi(m_wicFormat);

    getMetaInfo();
}

MGX::Coding::TextureFile::TextureFile(UINT width, UINT  height, GUID fmt) :
    m_width(width),
    m_height(height),
    m_wicFormat(fmt),
    m_dxgiFormat(ConvertGuidFormatToDxgi(fmt))
{
    createWicFactory();

    // Create bitmap
    MGX_EVALUATE_HRESULT("IWICImagingFactory2->CreateBitmap(...)", m_ptrWicFactory->CreateBitmap(width, height, fmt, WICBitmapCacheOnLoad, &m_ptrSrcBitmap));

    getMetaInfo();
}

MGX::Coding::TextureFile::TextureFile(UINT  width, UINT  height, DXGI_FORMAT fmt) :
    m_width(width),
    m_height(height),
    m_dxgiFormat(fmt),
    m_wicFormat(ConvertDxgiFormatToGuid(fmt))
{
    createWicFactory();

    // Create bitmap
    MGX_EVALUATE_HRESULT("IWICImagingFactory2->CreateBitmap(...)", m_ptrWicFactory->CreateBitmap(width, height, m_wicFormat, WICBitmapCacheOnLoad, &m_ptrSrcBitmap));

    getMetaInfo();
}

void MGX::Coding::TextureFile::createWicFactory()
{
    if (!m_ptrWicFactory)
    {
        MGX_EVALUATE_HRESULT("CoCreateInstance(...) for IID_IWICImagingFactory2", CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_ptrWicFactory)))
    }
}

bool MGX::Coding::TextureFile::ReadPixels(UINT x, UINT y, UINT width, UINT height, void* dstBuffer, UINT dstSize, UINT dstStride, DXGI_FORMAT requestedFormat)
{
    bool succeed = false;

    // Fix input
    if (requestedFormat == DXGI_FORMAT_UNKNOWN)
        requestedFormat = m_dxgiFormat;

    // Check if converter is required
    if (requestedFormat == m_dxgiFormat)
    {
        if (m_ptrSrcDecoder)
        {
            // Copy rect
            WICRect cpRect;
            cpRect.X = x;
            cpRect.Y = y;
            cpRect.Width = width;
            cpRect.Height = height;

            // Copy from decoder
            MGX_EVALUATE_HRESULT("IWICBitmapFrameDecode->CopyPixels(...)", m_ptrSrcDecoder->CopyPixels(&cpRect, dstStride, dstSize, (BYTE*)dstBuffer));
            succeed = true;
        }
        else if (m_ptrSrcBitmap)
        {
            // Copy rect
            WICRect cpRect;
            cpRect.X = x;
            cpRect.Y = y;
            cpRect.Width = width;
            cpRect.Height = height;

            // Copy from decoder
            MGX_EVALUATE_HRESULT("IWICBitmap->CopyPixels(...)", m_ptrSrcBitmap->CopyPixels(&cpRect, dstStride, dstSize, (BYTE*)dstBuffer));
            succeed = true;
        }
    }
    else
    {
        if (m_ptrSrcDecoder)
        {
            // Convert DXGI to GUID
            GUID targetFmt = ConvertDxgiFormatToGuid(requestedFormat);
            if (targetFmt != GUID({ 0 }))
            {
                // Create converter
                ComPointer<IWICFormatConverter> ptrConverter;
                MGX_EVALUATE_HRESULT("IWICImagingFactory2->CreateFormatConverter(...)", m_ptrWicFactory->CreateFormatConverter(&ptrConverter));

                // Check conversion
                BOOL canConvert = FALSE;
                MGX_EVALUATE_HRESULT("IWICFormatConverter->CanConvert(...)", ptrConverter->CanConvert(m_wicFormat, targetFmt, &canConvert));
                if (canConvert)
                {
                    MGX_EVALUATE_HRESULT("IWICFormatConverter->Initialize(...)", ptrConverter->Initialize(m_ptrSrcDecoder, targetFmt, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeMedianCut));

                    // Copy rect
                    WICRect cpRect;
                    cpRect.X = x;
                    cpRect.Y = y;
                    cpRect.Width = width;
                    cpRect.Height = height;

                    // Copy from converter
                    MGX_EVALUATE_HRESULT("IWICFormatConverter->CopyPixels(...)", ptrConverter->CopyPixels(&cpRect, dstStride, dstSize, (BYTE*)dstBuffer));
                    succeed = true;
                }
            }
        }
    }

    return succeed;
}

void MGX::Coding::TextureFile::getMetaInfo()
{
    // Get pixel format
    if (m_ptrSrcDecoder)
    {
        MGX_EVALUATE_HRESULT("IWICBitmapFrameDecode->GetPixelFormat(...)", m_ptrSrcDecoder->GetPixelFormat(&m_wicFormat));
        MGX_EVALUATE_HRESULT("IWICBitmapFrameDecode->GetSize(...)", m_ptrSrcDecoder->GetSize(&m_width, &m_height));
    }

    // Get component info3
    ComPointer<IWICComponentInfo> ptrCmpInfo;
    MGX_EVALUATE_HRESULT("IWICImagingFactory2->CreateComponentInfo(...)", m_ptrWicFactory->CreateComponentInfo(m_wicFormat, &ptrCmpInfo));

    // Query for fmt info
    ComPointer<IWICPixelFormatInfo> ptrFmtInfo;
    MGX_EVALUATE_HRESULT("ComPointer<IWICComponentInfo>::queryInterfaceHR(...) for ComPointer<IWICPixelFormatInfo>", ptrCmpInfo.queryInterfaceHR(ptrFmtInfo));

    // Get format details
    MGX_EVALUATE_HRESULT("IWICPixelFormatInfo->GetBitsPerPixel(...)", ptrFmtInfo->GetBitsPerPixel(&m_bpp));
    MGX_EVALUATE_HRESULT("IWICPixelFormatInfo->GetChannelCount(...)", ptrFmtInfo->GetChannelCount(&m_channelCount));
}

GUID MGX::Coding::TextureFile::ConvertDxgiFormatToGuid(DXGI_FORMAT fmt) noexcept
{
    // Find format while list valid
    GUID fmtOut = { 0 };
    for (const GuidDXGI* ptrFmt = s_equivalentFormats; ptrFmt->guid != GUID({ 0 }); ptrFmt++)
    {
        if (ptrFmt->dxgi == fmt)
        {
            fmtOut = ptrFmt->guid;
            break;
        }
    }
    
    return fmtOut;
}

DXGI_FORMAT MGX::Coding::TextureFile::ConvertGuidFormatToDxgi(GUID fmt) noexcept
{
    // Find format while list valid
    DXGI_FORMAT fmtOut = DXGI_FORMAT_UNKNOWN;
    for (const GuidDXGI* ptrFmt = s_equivalentFormats; ptrFmt->dxgi != DXGI_FORMAT(0); ptrFmt++)
    {
        if (ptrFmt->guid == fmt)
        {
            fmtOut = ptrFmt->dxgi;
            break;
        }
    }

    return fmtOut;
}
