#pragma once
#ifndef __WINDOWS_LOG_H__
#define __WINDOWS_LOG_H__


#include "Log.h"
#include "System\WindowsHeader.h"


namespace myengine {

	class WindowsLog: public Log
	{
	public:
			WindowsLog();
			virtual ~WindowsLog();

		virtual void	print(const char *string);


	private:
		void		createWindow();
		void		destroyWindow();

		HWND		hWnd;
		HWND		hwndEdit;
		HFONT		hFont;
		unsigned	charCount;
	};

}


#endif //!__WINDOWS_LOG_H__