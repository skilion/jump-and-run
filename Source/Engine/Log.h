#pragma once
#ifndef __LOG_H__
#define __LOG_H__


#include "Platform.h"

#include "File/File.h"


#define _TO_STRING(x) #x
#define TO_STRING(x) _TO_STRING(x)
#define AT __FILE__ "(line " TO_STRING(__LINE__) ")"

#define LOG(string) log->printf("%s - %s", string, AT);


namespace myengine {

	class Log
	{
	public:
			Log();
			virtual ~Log();

		static Log		*create();

		virtual void	print(const char *string) = 0;
		void			printf(const char *format, ...);


	private:
		File	*logFile;
		char	*tmpString;
	};

	extern Log	*log;
	
}


#endif //!__LOG_H__