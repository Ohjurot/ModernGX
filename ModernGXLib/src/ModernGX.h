#pragma once

// Setup windows
#define NOMINMAX
#define NOGDI
#define NOOPENFILE

// Include windows
#include <Windows.h>

// Windows enable visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Include DirectX 12
#include <dxgi1_6.h>
#include <d3d12.h>

// Windows releated stuff
#include <wincodec.h>

// STD LIB
#include <atomic>

// Include lib generic
#include <ModernGX/Util/ComPointer.h>
