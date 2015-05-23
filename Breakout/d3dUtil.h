//=======================================================================================
// d3dUtil.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

//#define DEBUG

#ifndef D3DUTIL_H
#define D3DUTIL_H

// Let VC++ know we are compiling for WinXP and up.  This let's us use
// API functions specific to WinXP (e.g., WM_INPUT API).
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600 // Vista
#endif

 
// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
	#define D3D_DEBUG_INFO
	#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#endif


#include <d3dx10.h>
#include <dxerr.h>
#include <cassert>


//*****************************************************************************
// Simple d3d error checker for book demos.
//*****************************************************************************

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 


//*****************************************************************************
// Convenience macro for releasing COM objects.
//*****************************************************************************

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }


//*****************************************************************************
// Convenience functions.
//*****************************************************************************

// Converts ARGB 32-bit color format to ABGR 32-bit color format.
D3DX10INLINE UINT ARGB2ABGR(UINT argb)
{
	BYTE A = (argb >> 24) & 0xff;
	BYTE R = (argb >> 16) & 0xff;
	BYTE G = (argb >>  8) & 0xff;
	BYTE B = (argb >>  0) & 0xff;

	return (A << 24) | (B << 16) | (G << 8) | (R << 0);
}

// Returns random float in [0, 1).
D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// Returns random float in [a, b).
D3DX10INLINE float RandF(float a, float b)
{
	return a + RandF()*(b-a);
}

// Returns random float in [a, b), by a min distance +r
D3DX10INLINE float RandF(float a, float b, float r)
{
	r = abs(r);
	float ret = a + RandF()*(b-a);
	if(ret > -r && ret < r) {
		if(ret < 0)
			return -r;
		else if(ret >= 0)
			return r;
	}
	return ret;
}

// Returns random vector on the unit sphere.
D3DX10INLINE D3DXVECTOR3 RandUnitVec3()
{
	D3DXVECTOR3 v(RandF(), RandF(), RandF());
	D3DXVec3Normalize(&v, &v);
	return v;
}
 
template<typename T>
D3DX10INLINE T Min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
D3DX10INLINE T Max(const T& a, const T& b)
{
	return a > b ? a : b;
}
 
template<typename T>
D3DX10INLINE T Lerp(const T& a, const T& b, float t)
{
	return a + (b-a)*t;
}

template<typename T>
D3DX10INLINE T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x); 
}


//*****************************************************************************
// Constants
//*****************************************************************************

const float INFINITY = FLT_MAX;
const float PI       = 3.14159265358979323;
const float MATH_EPS = 0.0001f;

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const D3DXCOLOR VIOLENTVIOLET(41/255.0,12/255.0,94/255.0,1);

const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);
const D3DXCOLOR GOLD(1.0f,0.8431372549f,0.0f,1.0f);
const D3DXCOLOR LIGHTWOOD(0.91764705882f,0.7725490196f,0.59607843137f, 1.0f);
const D3DXCOLOR MEDWOOD(0.71764705882f,0.49803921568f,0.25882352941f, 1.0f);
const D3DXCOLOR LIGHTCARD(0.8431372549f, 0.70588235294f, 0.54901960784f, 1.0f);
const D3DXCOLOR BOTTLEGREEN(0.0f,0.49411764705f,0.0f,1.0f);
const D3DXCOLOR WALLCOLOR(0.86666666666f,0.83921568627f,0.80784313725f,1.0f);

const D3DXCOLOR DARKBLUE(0.094f, 0.149f, 0.478f, 1.0f);
const D3DXCOLOR DARKISHBLUE(0.235f, 0.314f, 0.823f, 1.0f);
const D3DXCOLOR SURFACEBLUE(0.478f, 0.549f, 1.0f, 1.0f);
const D3DXCOLOR DARKGRAY(0.4235f, 0.4235f, 0.4235f, 1.0f);
const D3DXCOLOR LTEGRAY(0.824f, 0.824f, 0.824f, 1.0f);
const D3DXCOLOR AIR(0.831f, 0.831f, 1.0f, 1.0f);

#endif // D3DUTIL_H