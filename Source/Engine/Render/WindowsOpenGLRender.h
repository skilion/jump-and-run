#pragma once
#ifndef __WINDOWS_GL_RENDER_H__
#define __WINDOWS_GL_RENDER_H__


#include "OpenGLHeader.h"
#include "OpenGLRender.h"


namespace myengine {

	class WindowsOpenGLRender: public OpenGLRender
	{
	public:
			WindowsOpenGLRender();
			//virtual ~WindowsOpenGLRender();
			
		//Render interface
		virtual void	frameEnd();
		virtual void	setResolution(const Vec2i &resolution);

		//OpenGLRender interface
		virtual bool	createWindow();
		virtual void	destroyWindow();


	protected:
		virtual bool	isExtAvailable(const char *extension);


	private:
		void	initializeOpenGLExt();
		bool	initializeOpenGL();
		void	shutdownOpenGL();

		bool	createDummyWindow();
		
		void	clearExtensions();
		
		//Needed by windows
		HINSTANCE			hInstance;
		HWND				hWnd;
		HDC					hDC;
		HGLRC				hGLRC;

		//WGL extensions
		const char			*wglExtensions;
		bool				wgl_arb_multisample;
		PFNWGLGETEXTENSIONSSTRINGEXTPROC	wglGetExtensionsStringEXT;
		PFNWGLGETEXTENSIONSSTRINGARBPROC	wglGetExtensionsStringARB;
		PFNWGLSWAPINTERVALEXTPROC			wglSwapIntervalEXT;
		PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB;
	};

}


#endif //!__WINDOWS_GL_RENDER_H__