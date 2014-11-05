#include <X11/Xatom.h>

#include "Log.h"
#include "Memory.h"
#include "Platform.h"
#include "LinuxOpenGLRender.h"

#include "System/LinuxSystem.h"

using namespace myengine;


char * MAIN_WND_TITLE = "MyEngine";
int LinuxOpenGLRender::errorLevel = 0;


//=============================================================================
// Creates a LinuxOpenGLRender class
//=============================================================================
Render *Render::create()
{
	return new LinuxOpenGLRender;
}


//=============================================================================
// Constructor
//=============================================================================
LinuxOpenGLRender::LinuxOpenGLRender()
{
	display = 0;
	window = 0;
	context = 0;
}

//=============================================================================
// Destructor
//=============================================================================
//LinuxOpenGLRender::~LinuxOpenGLRender() {}

//=============================================================================
// Creates a window that supports OpenGL
//=============================================================================
bool LinuxOpenGLRender::createWindow()
{
	errorLevel = 0;
	XSetErrorHandler(overrideErrorHandler);

	display = XOpenDisplay(0);
	if(!display) {
		log->print("Can't open the X display");
		return false;
	}

	XSynchronize(display, false);

	log->printf(" X Server Vendor: %s", ServerVendor(display));
	log->printf(" X Server Release: %i", ServerVendor(display));

	int dummy;
	if(!glXQueryExtension(display, &dummy, &dummy)) {
		log->print("X server has no OpenGL GLX extension");
		return false;
	}

	int maj, min;
	glXQueryVersion(display, &maj, &min);
	log->printf(" GLX Extension version: %i.%i", maj, min);

	XF86VidModeQueryVersion(display, &maj, &min);
	log->printf(" XF86 VideoMode extension version: %i.%i", maj, min);

	screen = DefaultScreen(display);
	Window rootWindow = RootWindow(display, screen);

	int attribList[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE,
		depth,
		GLX_STENCIL_SIZE,
		4,
		None
	};

    XVisualInfo *visualInfo;
	visualInfo = glXChooseVisual(display, screen, attribList);
	if(!visualInfo) {
		log->print("Could not find a compatible visual");
		return false;
	}

	if(visualInfo->c_class != TrueColor) {
		log->print("TrueColor visual required for this program");
		return false;
	}

	context = glXCreateContext(display, visualInfo, 0, true);
	if(!context) {
		log->print("Can't create an OpenGL render context");
		return false;
	}

	Colormap colormap;
	colormap = XCreateColormap(display, rootWindow, visualInfo->visual, AllocNone);
	if(!colormap) {
		log->print("Could not create a colormap with the specified visual");
		return false;
	}

	XSetWindowAttributes attributes;
	attributes.background_pixel = BlackPixel(display, screen);
	attributes.border_pixel = 0;
	attributes.colormap = colormap;
	attributes.event_mask = ButtonPressMask | ButtonReleaseMask | FocusChangeMask |
		KeymapStateMask | KeyPressMask | KeyReleaseMask;
	attributes.override_redirect = False;
	unsigned valuemask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect;

	int x, y;
	if(fullscreen) {
		//attributes.override_redirect = True;
		x = y = 0;
	} else {
		x = (DisplayWidth(display, screen) - getResolution().getX()) / 2;
		y = (DisplayHeight(display, screen) - getResolution().getY()) / 2;
	}

	//log->printf("Creating a %ix%ix%i window...", getResolution().getX(), getResolution().getY(), depth);
	window = XCreateWindow(display, rootWindow, x, y, getResolution().getX(), getResolution().getY(),
				0, visualInfo->depth, InputOutput, visualInfo->visual,
				valuemask, &attributes);

	if(errorLevel > 0) {
		log->print("Could not create a window with the specified visual");
		return false;
	}

	linuxSystem->setWindow(display, window);

	//Window title
	XTextProperty title;
	XmbTextListToTextProperty(display, &MAIN_WND_TITLE, 1, XStringStyle, &title);

	//Ensure the window manager will respect windows position and size
	XSizeHints *normalHints = XAllocSizeHints();
	normalHints->flags = USPosition | PMinSize | PMaxSize;
	normalHints->min_width 	= getResolution().getX();
	normalHints->min_height	= getResolution().getY();
	normalHints->max_width	= getResolution().getX();
	normalHints->max_height	= getResolution().getY();

	//Ensure the window manager will give input to the window
	XWMHints *wmHints = XAllocWMHints();
	wmHints->flags = InputHint;
	wmHints->input = True;

	//Window class
	//XClassHint *classHint = XAllocClassHint();
	//classHint->res_name = "MyEngine";
	//classHint->res_class = "MyEngineClass";

	//Set window properties
	XSetWMProperties(display, window, &title, &title, 0, 0, normalHints, wmHints, /*classHint*/ 0);

	//Free memory
	//XFree(classHint);
	XFree(wmHints);
	XFree(normalHints);
	XFree(title.value);

	if(!glXMakeCurrent(display, window, context)) {
		log->print("Can't attach the OpenGL render context to the window");
		return false;
	}

	if(!glXIsDirect(display, context)) log->print("Warning: direct rendering is disabled !");

	//Show the window
	XMapRaised(display, window);

	if(fullscreen) {
		/*const Atom MOTIF_WM_HINTS = XInternAtom(display, "_MOTIF_WM_HINTS", False);
		if(MOTIF_WM_HINTS) {
			struct MotifWmHints {
				unsigned long	flags;
				unsigned long	functions;
				unsigned long	decorations;
				long			inputMode;
				unsigned long	status;
			} motifWmHints;
			motifWmHints.flags = 2; //MWM_HINTS_DECORATIONS
			motifWmHints.decorations = 0;

			XChangeProperty(display, window, MOTIF_WM_HINTS, MOTIF_WM_HINTS, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&motifWmHints), 5);
		} else log->print("Warning: the window manager does not support _MOTIF_WM_HINTS !");*/

		const Atom NET_WM_STATE = XInternAtom(display, "_NET_WM_STATE", False);
		const Atom NET_WM_STATE_FULLSCREEN = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

		if(!NET_WM_STATE) log->print("Warning: the window manager does not support _NET_WM_STATE !");
		else if(!NET_WM_STATE_FULLSCREEN) log->print("Warning: the window manager does not support _NET_WM_STATE_FULLSCREEN !");
		else {
			XEvent event;
			Memory::memset(&event, 0, sizeof(XEvent));
			event.xclient.type = ClientMessage;
			event.xclient.serial = 0;
			event.xclient.send_event = True;
			event.xclient.display = display;
			event.xclient.window = window;
			event.xclient.message_type = NET_WM_STATE;
			event.xclient.format = 32;
			event.xclient.data.l[0] = 1;
			event.xclient.data.l[1] = NET_WM_STATE_FULLSCREEN;
			event.xclient.data.l[2] = 0;
			event.xclient.data.l[3] = 0;
			event.xclient.data.l[4] = 0;
			event.xclient.data.l[5] = 0;

			if(!XSendEvent(display, rootWindow, False, SubstructureNotifyMask | SubstructureRedirectMask, &event)) {
				log->print("ERROR XSendEvent");
			}
		}

		/*Atom property = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
		Atom list[] = {XInternAtom(display, "_NET_WM_WINDOW_TYPE_SPLASH", False)};
		XChangeProperty(display, window, property, XA_ATOM, 32, PropModeReplace, (unsigned char*)list, 1);*/

		/*Atom retType;
		int retFormat;
		long unsigned count, bytesAfter;
		Atom *list;
		Atom property = XInternAtom(display, "_NET_SUPPORTED", False);
		if(!XGetWindowProperty(display, rootWindow, property, 32, 32, False, XInternAtom(display, "_NET_WM_STATE", False), &retType, &retFormat, &count, &bytesAfter, (unsigned char**)&list)) {
			log->print("XGetWindowProperty failed");
		} else {
			log->printf("retFormat %i count %i bytesAfter %i", retFormat, count, bytesAfter);
		}*/
		//XSetInputFocus(dislpay, window, RevertToParent, CurrentTime);
	}

	if(fullscreen) linuxSystem->setFullscreen(true);

	//XSetErrorHandler(0);

	return true;
}

//=============================================================================
// Destroy the window
//=============================================================================
void LinuxOpenGLRender::destroyWindow()
{
	glXMakeCurrent(display, 0, 0);

	glXDestroyContext(display, context);
	context = 0;

	XDestroyWindow(display, window);
	window = 0;

	screen = 0;

	XCloseDisplay(display);
	display = 0;
}

//=============================================================================
// Shows to the screen what we have drawn
//=============================================================================
void LinuxOpenGLRender::frameEnd()
{
	OpenGLRender::frameEnd();
	glXSwapBuffers(display, window);
}

//=============================================================================
// Returns the display pointer
//=============================================================================
/*Display *LinuxOpenGLRender::getDisplay()
{
	return display;
}

//=============================================================================
// Returns the window structure
//=============================================================================
Window LinuxOpenGLRender::getDeleteMessage()
{
	return wmDeleteMessage;
}*/

//=============================================================================
// Function to override the default X11 error handler
//=============================================================================
int LinuxOpenGLRender::overrideErrorHandler(Display *display, XErrorEvent *event)
{
	errorLevel = 1;
	log->print("X Error");
	char message[MAX_STRING];
	XGetErrorText(display, event->error_code, message, MAX_STRING);
	log->print(message);
	log->printf("(Request code: %i Minor code: %i)", event->request_code, event->minor_code);
	return 0;
}
