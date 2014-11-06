#include <ctime>
#include <cstdio>
#include <cstdarg>

#include "Log.h"
#include "myString.h"
#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Log::Log()
{
	logFile		= File::createFile();
	tmpString	= new char[MAX_STRING];

	logFile->open("engine.log", File::OPENMODE_OUTPUT);
}

//=============================================================================
// Destructor
//=============================================================================
Log::~Log()
{
	logFile->close();

	delete logFile;
	delete [] tmpString;
}

//=============================================================================
// Prints a string to the log file
//=============================================================================
void Log::print(const char *string)
{
	if(!string) return;

	time_t rawTime;
	time(&rawTime);
	tm *timeInfo = localtime(&rawTime);

	char timeString[32];
	strftime(timeString, sizeof(timeString), "[%d-%b-%Y %H:%M:%S] ", timeInfo);

	logFile->writeString(timeString);
	logFile->writeLine(string);
	logFile->flush();
}

//=============================================================================
// Prints a formatted string to the log file
//=============================================================================
void Log::printf(const char *format, ...)
{
	if(!format) return;

	va_list list;
	va_start(list, format);
	vsprintf(tmpString, format, list);
	va_end(list);

	print(tmpString);
}
