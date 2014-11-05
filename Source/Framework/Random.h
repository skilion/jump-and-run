#pragma once
#ifndef __RANDOM_H__
#define __RANDOM_H__


//=============================================================================
//
//		Linear congruential generator
//
//=============================================================================


namespace myengine {
	
	class Random
	{
	public:
			Random();
			Random(unsigned seed);
			//~Random();

		void		initialize(unsigned seed);

		bool		getBool();					// true\false
		int			getInt();					// 0 <= x <= 2147483647
		int			getInt(int max);			// 0 <= x <= max
		unsigned	getUnsigned();				// 0 <= x <= 4294967295
		unsigned	getUnsigned(unsigned max);	// 0 <= x <= max
		float		getFloat();					// 0.0 <= x <= 1.0
		

	private:
		unsigned	next;
	};

	extern Random random;

}


#endif //!__RANDOM_H__