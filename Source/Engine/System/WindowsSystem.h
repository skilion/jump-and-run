#pragma once
#ifndef __WINDOWS_SYSTEM_H__
#define __WINDOWS_SYSTEM_H__


#include "System.h"
#include "WindowsHeader.h"


namespace myengine {

	class WindowsSystem: public System
	{
	public:
			WindowsSystem();
			virtual ~WindowsSystem();

		//System interface
		virtual const char	*getClipboardText();
		virtual unsigned	getFreeSpaceMegaBytes();
		virtual unsigned	getMilliseconds();
		virtual void		getDesktopInfo(int &width, int &height, int &colors);
		virtual void		setClipboardText(const char *text);
		virtual void		setCursor(Cursor cursor);
		virtual void		setMouseLocked(bool mouseLocked);
		virtual void		messageBox(const char *text, const char *caption);
		virtual void		handleEvents();
		virtual void		pauseTillExit();
		virtual void		sleep(unsigned milliseconds);

		void				setMainWindow(HWND hWnd);

	
	private:
		void			updateMouse();

		LARGE_INTEGER	frequency, start;
		WSADATA			wsaData;
		HWND			mainWindow;
		bool			mouseLocked;
		HCURSOR			cursors[CURSOR_LAST];
	};

	extern WindowsSystem *windowsSystem;
}


#endif //!__WINDOWS_SYSTEM_H__
