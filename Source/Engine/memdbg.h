#pragma once
#ifndef __MEMDBG_H__
#define __MEMDBG_H__


//#include "Platform.h"


// Every cpp file must include this file for memory leaks detection purpose.
// Also this file must be the latest inclusion for the cpp file to avoid name collisions 

/*#undef new
#undef delete
#undef DBG_NEW
#undef DBG_DELETE*/

#ifdef _DEBUG
	#ifdef DONT_USE_MEM_MGR
		/*#ifdef WIN32
			#define _CRTDBG_MAP_ALLOC
			#include <stdlib.h>
			#include <crtdbg.h>

			#ifndef DEBUG_NEW
			#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
			#define new DEBUG_NEW
			#endif

			//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
			#undef free
		#else
			#error Memory leaks detection on this platform won't be performed (better enable the built-in memory manager)
		#endif //WIN32*/
	#else
		///*extern*/ void *operator new(unsigned size, const char *origin);
		///*extern*/ void *operator new[](unsigned size, const char *origin);
		///*extern*/ void operator delete(void *address, const char *origin);
		///*extern*/ void operator delete[](void *address, const char *origin);

		//#define TO_STR2(s) #s
		//#define TO_STR(s) TO_STR2(s)
		//#define new new(__FILE__ "(line " TO_STR(__LINE__) ")")

		/*#define DBG_NEW new((const char *)(__FILE__))
		#define new DBG_NEW*/

		/*#define DBG_DELETE delete((const char *)(__FILE__))
		#define delete DBG_DELETE*/
	#endif //DONT_USE_MEM_MGR
#endif //_DEBUG


#endif //!__MEMDBG_H__