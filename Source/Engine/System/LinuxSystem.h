#pragma once
#ifndef __LINUX_SYSTEM_H__
#define __LINUX_SYSTEM_H__


#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

#include "System.h"

#include "EventSystem/KeyEvent.h"


namespace myengine {

	class LinuxSystem: public System
	{
	public:
			LinuxSystem();
			//virtual ~LinuxSystem();

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

		void		        setWindow(Display *display, Window window);
		void				setFullscreen(bool fullscreen); //public for test


    private:
		KeyEvent::KeyCode	translateKey(int keyCode);
		void                updateMouse();

        unsigned	startTime;
        Display		*display;
        int         screen;
        Window      rootWindow, mainWindow;
        Atom		wmDeleteMessage;
        bool		mouseLocked;
		bool		mouseResetted;	//True if the mouse has been positioned at center
		XF86VidModeModeInfo		desktopMode, fullscreenMode;
		
		::Cursor	cursors[CURSOR_LAST];
	};

	extern LinuxSystem *linuxSystem;

}


#endif //!__LINUX_SYSTEM_H__
