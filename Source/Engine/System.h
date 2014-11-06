#pragma once


namespace myengine {

	class System
	{
	public:
		enum Cursor {
			CURSOR_NONE,
			CURSOR_ARROW,
			CURSOR_HAND,
			CURSOR_SIZENESW,
			CURSOR_SIZENS,
			CURSOR_SIZENWSE,
			CURSOR_SIZEWE,
			
			CURSOR_LAST
		};
			//System();
			virtual ~System();
			
		static System		*create();

		virtual const char	*getClipboardText() = 0;
		virtual unsigned	getFreeSpaceMegaBytes() = 0;
		virtual unsigned	getMilliseconds() = 0;
		virtual void		getDesktopInfo(int &width, int &height, int &colors) = 0;

		virtual void		setClipboardText(const char *text) = 0;
		virtual void		setCursor(Cursor cursor) = 0;
		virtual void		setMouseLocked(bool mouseLocked) = 0;

		virtual void		messageBox(const char *text, const char *caption) = 0;
		virtual void		handleEvents() = 0;
		virtual void		pauseTillExit() = 0;
		
		virtual void		sleep(unsigned milliseconds) = 0;
	};

	extern System	*system;
	
}
