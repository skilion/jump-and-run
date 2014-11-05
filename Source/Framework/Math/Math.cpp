#include <cmath>

#include "Math.h"
#include "Platform.h"

using namespace myengine;


//=============================================================================
// Returns the arc cosine of n
//=============================================================================
float myengine::acos(float n)
{
	return std::acos(n);
}

//=============================================================================
// Returns the arc sine of n
//=============================================================================
float myengine::asin(float n)
{
	return std::asin(n);
}

//=============================================================================
// Returns the arc tangent of n
//=============================================================================
float myengine::atan(float n)
{
	return std::atan(n);
}

//=============================================================================
// Returns the cosine of n
//=============================================================================
float myengine::cos(float n)
{
#ifdef ASM_SSE2
	__asm {
		fld		dword ptr [n]
		fcos
	}
#else
	return std::cos(n);
#endif
}

//=============================================================================
// Returns true if the specified number is a power of 2
//=============================================================================
bool myengine::isPow2(unsigned n)
{
	return (n != 0) && ((n & (n - 1)) == 0);
}

//=============================================================================
// Returns the next nearest power of 2 of the specified number
//=============================================================================
unsigned myengine::nextPow2(unsigned n)
{
	register unsigned p = 1;
	while(p < n) {
		p <<= 1;
	}
	return p;
}

//=============================================================================
// Returns the square root of the specified number
//=============================================================================
float myengine::sqrt(float n)
{
#ifdef ASM_SSE2
	__asm {
		fld		dword ptr [n]
		fsqrt
		/*movupd		xmm0, n
		sqrtpd		xmm0, xmm0
		movupd		dword ptr [n], xmm0
		fld			dword ptr [n]*/

	}
#else
	return std::sqrt(n);
#endif
}

//=============================================================================
// Returns the sine of n
//=============================================================================
float myengine::sin(float n)
{
#ifdef ASM_SSE2
	__asm {
		fld		dword ptr [n]
		fsin
	}
#else
	return std::sin(n);
#endif
}


//=============================================================================
// Gets the sinus and cosinus of n
//=============================================================================
void myengine::sincos(float n, float &s, float &c)
{
	s = std::sin(n);
	c = std::cos(n);
}

//=============================================================================
// Returns the tangent of n
//=============================================================================
float myengine::tan(float n)
{
#ifdef ASM_SSE2
	__asm {
		fld		dword ptr [n]
		fptan
		fincstp
	}
#else
	return std::sin(n);
#endif
}
