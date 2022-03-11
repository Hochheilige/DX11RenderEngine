#pragma once

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

