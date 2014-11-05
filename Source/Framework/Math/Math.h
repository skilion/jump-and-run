#pragma once
#ifndef __MATH_H__
#define __MATH_H__


//FIX
#undef FLT_EPSILON
#undef min
#undef max


namespace myengine {

	const float PI			= 3.14159265f;
	const float TWO_PI		= PI * 2;
	const float DEG2RAD		= PI / 180.0f;
	const float RAD2DEG		= 180.0f / PI;
	const float FLT_EPSILON	= 1.19209289e-7f;
	const float SQRT_2		= 1.41421356f;
	

	float		acos(float n);
	float		asin(float n);
	float		atan(float n);
	float		cos(float n);
	bool		isPow2(unsigned n);
	unsigned	nextPow2(unsigned n);
	float		sqrt(float n);
	float		sin(float n);
	void		sincos(float n, float &s, float &c);
	float		tan(float n);
	
	//=====================================================================
	// Returns the absolute value the specified number
	//=====================================================================
	template<class T>
	inline const T abs(T n) {
		return n < 0 ? -n : n;
	}

	//=============================================================================
	// Returns the cube of n
	//=============================================================================
	template<class T>
	inline T cube(T n)
	{
		return n * n * n;
	}

	//=============================================================================
	// Returns true if the given floats are equals
	//=============================================================================
	inline bool fequal(float n1, float n2)
	{
		return abs<float>(n1 - n2) < FLT_EPSILON;
	}

	//=============================================================================
	// Returns true if the given float is zero
	//=============================================================================
	inline bool fzero(float n)
	{
		return abs<float>(n) < FLT_EPSILON;
	}

	//=====================================================================
	// Returns the greater of n1 and n2
	//=====================================================================
	template<class T>
	inline T max(T n1, T n2) {
		return (n1 > n2) ? n1 : n2;
	}

	//=============================================================================
	// Returns the lesser of n1 and n2
	//=============================================================================
	template<class T>
	inline T min(T n1, T n2)
	{
		return (n1 > n2) ? n2 : n1;
	}

	//=============================================================================
	// Returns base raised to the power exponent
	//=============================================================================
	template<class T>
	T pow(T base, int exponent)
	{
		if(exponent == 0) return 1;

		bool negative = false;
		if(exponent < 0) {
			negative = true;
			exponent = -exponent;
		}

		T pow = base;
		while(--exponent) pow *= base;

		return negative ? (1 / pow) : pow;
	}

	//=============================================================================
	// Returns the square root of n
	//=============================================================================
	template<class T>
	inline T square(T n)
	{
		return n * n;
	}

}


#endif //!__MATH_H__