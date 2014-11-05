#pragma once
#ifndef __LINUX_GL_RENDER_H__
#define __LINUX_GL_RENDER_H__


#include <X11/Xlib.h>

#include "OpenGLHeader.h"
#include "OpenGLRender.h"


namespace myengine {

	class LinuxOpenGLRender: public OpenGLRender
	{
	public:
			LinuxOpenGLRender();
			//virtual ~LinuxOpenGLRender();

		virtual bool	createWindow();
		virtual void	destroyWindow();

		virtual void	frameEnd();


	protected:
		//virtual bool	isExtAvailable(const char *extension);


	private:
		//Display		*getDisplay();
		//Atom		getDeleteMessage();

		Display		*display;
		int			screen;
		Window		window;
		GLXContext	context;

		static int	overrideErrorHandler(Display *display, XErrorEvent *event);

		static int	errorLevel;
	};

}


#endif //!__LINUX_GL_RENDER_H__
