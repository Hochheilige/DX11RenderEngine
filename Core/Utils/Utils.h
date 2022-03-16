#pragma once

#include <WndDxIncludes.h>

#if defined(DEBUG) | defined(_DEBUG)

#ifndef DXCHECK
#define DXCHECK(x)                                                   \
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                       \
	}
#endif

#else

#define DXCHECK(x) x

#endif

ID3DBlob* CompileShader(const wchar_t* filename, const D3D_SHADER_MACRO* defines,
	const char* entrypoint, const char* target);
