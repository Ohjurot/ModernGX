#pragma once

// Setup windows
#define NOMINMAX
#define NOGDI
#define NOOPENFILE

// Include windows
#include <Windows.h>

// Include DirectX 12
#include <dxgi1_6.h>
#include <d3d12.h>

// Windows releated stuff
#include <wincodec.h>

// STD LIB
#include <atomic>

// Include lib generic
#include <ModernGX/Util/ComPointer.h>

// MGX Defines

// MGX_INIT_CORE Macro
#define MGX_INIT_CORE() 

// MGX_INIT_DEBUG Macro
#define MGX_INIT_DEBUG() \
ComPointer<ID3D12Debug> __mgx_init_debug_ptr; \
if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&__mgx_init_debug_ptr)))) { \
    __mgx_init_debug_ptr->EnableDebugLayer(); \
} 

// MGX_INIT() Macro
#ifdef MGX_DEBUG
#define MGX_INIT() \
MGX_INIT_CORE() \
MGX_INIT_DEBUG() 
#else
#define MGX_INIT() \
MGX_INIT_CORE()
#endif

// MGX_COM_NAME(comPtr) Macro
#ifdef MGX_DISABLE_INTERNAL_D3D_NAMEING
#define MGX_COM_NAME(ptr, nm)
#else
#define MGX_COM_NAME(ptr, nm) ptr.name(nm)
#endif
