#include "Log.h"
#include "Engine.h"
#include "Memory.h"
#include "Render.h"
#include "myString.h"
//#include "..\resource.h"
#include "WindowsOpenGLRender.h"

#include "System/WindowsSystem.h"
#include "System/WindowsWndProc.h"

#include "memdbg.h"

using namespace myengine;


//Constants for the main window
const char * const MAIN_WND_CLASS = "MyEngineClass";
const char * const MAIN_WND_TITLE = "MyEngine";


//=============================================================================
// Creates the Render class
//=============================================================================
Render *Render::create()
{
	return new WindowsOpenGLRender();
}


//=============================================================================
// Constructor
//=============================================================================
WindowsOpenGLRender::WindowsOpenGLRender()
{
	hInstance = 0;
	hWnd = 0;
	hDC = 0;
	hGLRC = 0;
}

//=============================================================================
// Destructor
//=============================================================================
//WindowsOpenGLRender::~WindowsOpenGLRender() {}

//=============================================================================
// Shows to the screen what we have drawn
//=============================================================================
void WindowsOpenGLRender::frameEnd()
{
	OpenGLRender::frameEnd();
	SwapBuffers(hDC);
}

//=============================================================================
// Sets the resolution
//=============================================================================
void WindowsOpenGLRender::setResolution(const Vec2i &resolution)
{
	if(hWnd) {
		Vec2i oldResolution = getResolution();
		OpenGLRender::setResolution(resolution);
		if(!createWindow()) {
			OpenGLRender::setResolution(oldResolution);
		}
	} else {
		OpenGLRender::setResolution(resolution);
	}
}

//=============================================================================
// Creates an OpenGL window
//=============================================================================
bool WindowsOpenGLRender::createWindow()
{
	if(!hInstance) hInstance = GetModuleHandle(0);

	if(createDummyWindow()) initializeOpenGLExt();
	destroyWindow();

	WNDCLASSEX wc;
	Memory::memset(&wc, 0, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= mainWndProc;
	wc.hInstance		= hInstance;
	//wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.lpszClassName	= MAIN_WND_CLASS;

	if(!RegisterClassEx(&wc)) {
		log->print("Can't register the window class");
		return false;
	}
	
	//log->printf("Creating a %ix%ix%i window...", resolution.getX(), resolution.getY(), colors);

	//Window style
	DWORD exstyle = 0;
	DWORD style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
	if(fullscreen) {
		style = WS_POPUP;
		exstyle = WS_EX_TOPMOST;
	}

	//Adjust the window size
	RECT rect;
	SetRect(&rect, 0, 0, getResolution().getX(), getResolution().getY());
	AdjustWindowRectEx(&rect, style, 0, exstyle);

	style |= WS_VISIBLE /*| WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	
	//Create the window
	hWnd = CreateWindowEx(exstyle,
		MAIN_WND_CLASS,
		MAIN_WND_TITLE,
		style, x, y, width, height,
		0, 0, 0, 0);

	if(!hWnd) {
		log->print("Can't create the main window");
		return false;
	}

	windowsSystem->setMainWindow(hWnd);

	hDC = GetDC(hWnd);
	if(!hDC) {
		log->print("Can't retrieve the device context");
		return false;
	}

	if(!initializeOpenGL()) return false;

	//Show the window
	//AnimateWindow(hWnd, 400, AW_ACTIVATE | AW_SLIDE);

	return true;
}

//=============================================================================
// Destroys the window
//=============================================================================
void WindowsOpenGLRender::destroyWindow()
{
	shutdownOpenGL();

	ReleaseDC(hWnd, hDC);
	hDC = 0;

	DestroyWindow(hWnd);
	windowsSystem->setMainWindow(0);
	hWnd = 0;

	UnregisterClass(MAIN_WND_CLASS, hInstance);
}

//=============================================================================
// Returns true if a given wgl extension is available
//=============================================================================
bool WindowsOpenGLRender::isExtAvailable(const char *extension)
{
	bool available = OpenGLRender::isExtAvailable(extension);
	if(available || String::strstr(wglExtensions, extension)) {
		return true;
	}

	log->printf("OpenGL extension not available: %s", extension);
	return false;
}

//=============================================================================
// Finds all WGL extensions we need
//=============================================================================
void WindowsOpenGLRender::initializeOpenGLExt()
{
	clearExtensions();

	//log->print("Probing WGL extensions...");

	wglGetExtensionsStringEXT = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGEXTPROC>(wglGetProcAddress("wglGetExtensionsStringEXT"));
	wglGetExtensionsStringARB = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(wglGetProcAddress("wglGetExtensionsStringARB"));
	if(!wglGetExtensionsStringEXT && !wglGetExtensionsStringARB) {
		log->print("Neither wglGetExtensionsStringEXT nor wglGetExtensionsStringARB found !");
		return;
	}

	//Retrieve the wgl extensions
	if(wglGetExtensionsStringARB) {
		wglExtensions = wglGetExtensionsStringARB(hDC);
		if(!wglExtensions) log->print("wglGetExtensionsStringARB returned 0 !");
	}
	if(!wglExtensions && wglGetExtensionsStringEXT) {
		wglExtensions = wglGetExtensionsStringEXT();
		if(!wglExtensions) log->print("wglGetExtensionsStringEXT returned 0 !");
	}
	if(!wglExtensions) return;

	//Vsync
	if(isExtAvailable("WGL_EXT_swap_control")) {
		wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
	}
	
	//wglChoosePixelFormatARB()
	if(isExtAvailable("WGL_ARB_pixel_format")) {
		wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
	}

	//Multisampling
	if(isExtAvailable("WGL_ARB_multisample")) wgl_arb_multisample = true;
}

//=============================================================================
// Initializes OpenGL
//=============================================================================
bool WindowsOpenGLRender::initializeOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	Memory::memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion		= 1;
	pfd.dwFlags			= PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED;
	pfd.iPixelType		= PFD_TYPE_RGBA; 
	pfd.cColorBits		= colors;
	pfd.cDepthBits		= depth;
	pfd.cStencilBits	= 4;
	
	int pixelFormat;
	if(wglChoosePixelFormatARB) {
		int attributes[32];
		Memory::memset(attributes, 0, sizeof(attributes));
		
		register unsigned n = 0;
		attributes[n++] = WGL_DRAW_TO_WINDOW_ARB;
		attributes[n++] = true;
		attributes[n++] = WGL_ACCELERATION_ARB; //not supported everywhere
		attributes[n++] = WGL_FULL_ACCELERATION_ARB; /*WGL_NO_ACCELERATION_ARB, WGL_GENERIC_ACCELERATION_ARB*/
		attributes[n++] = WGL_SUPPORT_OPENGL_ARB;
		attributes[n++] = true;
		attributes[n++] = WGL_DOUBLE_BUFFER_ARB;
		attributes[n++] = true;
		attributes[n++] = WGL_PIXEL_TYPE_ARB;
		attributes[n++] = WGL_TYPE_RGBA_ARB;
		attributes[n++] = WGL_COLOR_BITS_ARB;
		attributes[n++] = colors;
		attributes[n++] = WGL_DEPTH_BITS_ARB;
		attributes[n++] = depth;
		attributes[n++] = WGL_STENCIL_BITS_ARB;
		attributes[n++] = 4;

		//Multisampling
		if(multisamples > 0) {
			if(wgl_arb_multisample) {
				attributes[n++] = WGL_SAMPLE_BUFFERS_ARB;
				attributes[n++] = 1;
				attributes[n++] = WGL_SAMPLES_ARB;
				attributes[n++] = multisamples;
			} else {
				log->printf("Multisampling is not supported by your videocard");
				multisamples = 0;
			}
		}
		
		unsigned numFormats;
		wglChoosePixelFormatARB(hDC, attributes, 0, 1, &pixelFormat, &numFormats);
		if(0 == numFormats) {
			log->print("Can't find an appropriate pixel format");
			return false;
		}
	}
	else {
		pixelFormat = ChoosePixelFormat(hDC, &pfd);
		if(!pixelFormat) {
			log->print("Can't find an appropriate pixel format");
			return false;
		}
	}

	if(!SetPixelFormat(hDC, pixelFormat, &pfd)) {
		log->print("Can't set the specified pixel format");
		return false;
	}

	hGLRC = wglCreateContext(hDC);
	if(!hGLRC) {
		log->print("Can't create a new OpenGL render context");
		return false;
	}

	if(!wglMakeCurrent(hDC, hGLRC)) {
		log->print("Can't set active the OpenGL render context");
		return false;
	}
	
	if(wglSwapIntervalEXT) wglSwapIntervalEXT(vsync);

	return true;
}

//=============================================================================
// Shutdowns OpenGL
//=============================================================================
void WindowsOpenGLRender::shutdownOpenGL()
{
	wglMakeCurrent(0, 0);
	wglDeleteContext(hGLRC);
	hGLRC = 0;
}

//=============================================================================
// Creates dummy window with OpenGL support
//=============================================================================
bool WindowsOpenGLRender::createDummyWindow()
{
	WNDCLASS wc;
	Memory::memset(&wc, 0, sizeof(WNDCLASS));
	//wc.hInstance		= hInstance;
	//wc.style			= CS_OWNDC;
	wc.lpfnWndProc		= DefWindowProc;
	wc.lpszClassName	= MAIN_WND_CLASS;
	if(!RegisterClass(&wc)) {
		log->print("Can't register the window class");
		return false;
	}
	
	hWnd = CreateWindow(MAIN_WND_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if(!hWnd) {
		log->print("Can't create the dummy window");
		return false;
	}

	hDC = GetDC(hWnd);
	
	PIXELFORMATDESCRIPTOR pfd;
	Memory::memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	//pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	//pfd.nVersion	= 1;
	//pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if(!pixelFormat) {
		log->print("Can't find an appropriate pixel format for the dummy window");
		return false;
	}

	if(!SetPixelFormat(hDC, pixelFormat, &pfd)) {
		log->print("Can't set the specified pixel format for the dummy window");
		return false;
	}

	hGLRC = wglCreateContext(hDC);
	if(!hGLRC) {
		log->print("Can't create a new OpenGL render context for the dummy window");
		return false;
	}

	if(!wglMakeCurrent(hDC, hGLRC)) {
		log->print("Can't set active the OpenGL render context of the dummy window");
		return false;
	}

	return true;
}

//=============================================================================
// Clear all OpenGL extensions
//=============================================================================
void WindowsOpenGLRender::clearExtensions()
{
	wglExtensions = 0;
	wgl_arb_multisample = 0;
	wglGetExtensionsStringEXT = 0;
	wglGetExtensionsStringARB = 0;
	wglSwapIntervalEXT = 0;
	wglChoosePixelFormatARB = 0;
}