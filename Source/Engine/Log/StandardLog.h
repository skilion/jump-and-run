#pragma once
#ifndef __STANDARD_LOG_H__
#define __STANDARD_LOG_H__



//=============================================================================
//
//		Implementation of the Log Class using the standard C++ IO
//
//=============================================================================


#include "Log.h"


namespace myengine {

	class StandardLog: public Log
	{
	public:
			//StandardLog();
			//virtual ~StandardLog();

		virtual void	print(const char *string);
	};

}


#endif //!__STANDARD_LOG_H__
