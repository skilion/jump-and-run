#pragma once
#ifndef __MEMORY_H__
#define __MEMORY_H__


namespace myengine {

	class Memory
	{
	public:
		static void		*memcpy(void *destination, const void *source, unsigned num);
		static int		memcmp(const void *ptr1, const void *ptr2, unsigned num);
		static void		*memset(void *ptr1, int value, unsigned num);

		//static void		*memcpy4B(void *destination, const void *source, unsigned num);
	};
	
	//Avoid name collision
	/*#define memcpy Memory::memcpy
	#define memcmp Memory::memcmp
	#define memset Memory::memset*/

}


#endif //!__MEMORY_H__