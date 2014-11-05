#include <ctime>
#include <cstdio>
#include <sys/time.h>

#include <X11/Xlib.h>
//#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <Memory.h>

#include "EventSystem/CharEvent.h"
#include "EventSystem/KeyEvent.h"
#include "EventSystem/MouseEvent.h"
#include "EventSystem/Sender.h"
#include "Render/Render.h"

#include "Log.h"
#include "Engine.h"
#include "LinuxSystem.h"

#include "memdbg.h"

using namespace myengine;


LinuxSystem *myengine::linuxSystem = 0;


//=============================================================================
// Creates a LinuxSystem class
//=============================================================================
System *System::create()
{
	if(!linuxSystem) {
		linuxSystem = new LinuxSystem;
	}

	return linuxSystem;
}


//=============================================================================
// Constructor
//=============================================================================
LinuxSystem::LinuxSystem()
{
	startTime = 0;
	display = 0;

	/*timespec t;
	t.tv_sec = 0;
	t.tv_nsec = 0;
	clock_settime(CLOCK_PROCESS_CPUTIME_ID, &t);*/
	startTime = getMilliseconds();
}

//=============================================================================
// Destructor
//=============================================================================
//LinuxSystem::~LinuxSystem() {}

//=============================================================================
// Returns the cliboard content
//=============================================================================
const char *LinuxSystem::getClipboardText()
{
    return 0;
}

//=============================================================================
// Returns the disk free space
//=============================================================================
unsigned LinuxSystem::getFreeSpaceMegaBytes()
{
	//TODO
	return 0;
}

//=============================================================================
// Returns the time in milliseconds since program has started
//=============================================================================
unsigned LinuxSystem::getMilliseconds()
{
    timeval t;
    gettimeofday(&t, 0); //getitimer ?
    return t.tv_sec * 1000 + t.tv_usec / 1000 - startTime;

	/*timespec t;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	return t.tv_sec * 1000 + t.tv_nsec / 1000000;*/
}

//=============================================================================
// Returns some desktop informations
//=============================================================================
void LinuxSystem::getDesktopInfo(int &width, int &height, int &colors)
{
	Display *display = XOpenDisplay(0);
	if(!display) {
		log->print("Can't open the X display, using default values for config...");
		width = 640;
		height = 480;
		colors = 24;
		return;
	}

	int screen = DefaultScreen(display);
	width = DisplayWidth(display, screen);
	height = DisplayHeight(display, screen);
	colors = DisplayPlanes(display, screen);

	XCloseDisplay(display);
}

//=============================================================================
// Sets the contents of the clipboard
//=============================================================================
void LinuxSystem::setClipboardText(const char *text)
{
	//TODO
}

//=============================================================================
// Sets the system cursor
//=============================================================================
void LinuxSystem::setCursor(Cursor cursor)
{
	XDefineCursor(display, mainWindow, cursors[cursor]);
}

//=============================================================================
// Lock the mouse at the center of the screen
//=============================================================================
void LinuxSystem::setMouseLocked(bool mouseLocked)
{
    this->mouseLocked = mouseLocked;
	mouseResetted = false;
}

//=============================================================================
// Alert the user
//=============================================================================
void LinuxSystem::messageBox(const char *text, const char *caption)
{
	log->print("=============================================================================");
	log->print(caption);
	log->print(text);
	log->print("=============================================================================");
}

//=============================================================================
// Pause till user press enter
//=============================================================================
void LinuxSystem::pauseTillExit()
{
    getchar();
}

//=============================================================================
// Handles X11 events
//=============================================================================
void LinuxSystem::handleEvents()
{
	XEvent event;
	KeySym keySym;

	KeyEvent keyEvent(KeyEvent::K_UNKNOW, KeyEvent::KEY_RELEASE);

	while(XPending(display)) {
		XNextEvent(display, &event);

		switch(event.type)
		{
        case ButtonPress:
		case ButtonRelease:
			keyEvent = KeyEvent(
				translateKey(event.xbutton.button),
				(event.type == ButtonPress) ? KeyEvent::KEY_PRESS : KeyEvent::KEY_RELEASE
			);
			keyEventSender.sendEvent(keyEvent);
			break;

        case ClientMessage:
        	if(static_cast<unsigned>(event.xclient.data.l[0]) == wmDeleteMessage) {
				if(render->isFullscreen()) setFullscreen(false);
                engine->exit();
            }
            break;

		case FocusIn:
			log->print("FocusIn");
			mouseResetted = false;
			engine->setPause(false);
			/*if(render->isFullscreen()) {
				setFullscreen(true);
			}*/
			break;

        case FocusOut:
        	log->print("FocusOut");
            engine->setPause(true);

            if(render->isFullscreen()) {
				//setFullscreen(false);
				//XLowerWindow(display, mainWindow);
				XIconifyWindow(display, mainWindow, screen);
            }
			break;

		case KeymapNotify:
			XRefreshKeyboardMapping(&event.xmapping);
			break;

		case KeyPress:
		case KeyRelease:
			char c;
			int len = XLookupString(&event.xkey, &c, 1, &keySym, 0);

			keyEvent = KeyEvent(translateKey(keySym),
				(KeyPress == event.type) ? KeyEvent::KEY_PRESS : KeyEvent::KEY_RELEASE);

			/*if((event.xkey.state & Mod1Mask) && (event.type == KeyRelease)) {
				switch(keySym)
				{
				case XK_Tab:
					//if(render->isFullscreen()) {
					log->print("ALT+TAB");
						setFullscreen(false);
						//XIconifyWindow(display, mainWindow, screen); // send FocusIn !
						XLowerWindow(display, mainWindow); //does not work everywere
					//}
					break;

				case XK_F4:
					log->print("ALT+F4");
					//if(render->isFullscreen()) {
						setFullscreen(false);
						engine->exit();
					//}
					break;
				}
			}*/

			keyEventSender.sendEvent(keyEvent);

			if((1 == len) && (KeyPress == event.type)) {
				CharEvent charEvent(c);
				charEventSender.sendEvent(charEvent);
			}
			break;
		}
	}

	updateMouse();
}

//=============================================================================
// Sets the X11 display
//=============================================================================
void LinuxSystem::setWindow(Display *display, Window window)
{
	this->display = display;
	this->mainWindow = window;

	screen = DefaultScreen(display);
	rootWindow = RootWindow(display, screen);

	wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, mainWindow, &wmDeleteMessage, 1);

	Memory::memset(&desktopMode, 0, sizeof(XF86VidModeModeInfo));
	Memory::memset(&fullscreenMode, 0, sizeof(XF86VidModeModeInfo));

	if(render->isFullscreen()) {
		int modeCount, bestMode = -1;
		XF86VidModeModeInfo **modes;
		XF86VidModeGetAllModeLines(display, screen, &modeCount, &modes);

		//First mode is desktop mode
		desktopMode = **modes;

		Vec2i resolution;
		resolution = render->getResolution();

		for(int i = 0; i < modeCount; i++) {
			if((modes[i]->hdisplay == resolution.getX()) && (modes[i]->vdisplay == resolution.getY())) {
				bestMode = i;
			}
		}

		if(bestMode < 0) {
			log->print("Can't find the desired fullscreen mode");
			//Todo stop app
		} else {
			fullscreenMode = *(modes[bestMode]);
		}

		XFree(modes);
	}
	
	//Create an invisible cursor
	XColor color;
	char empty[] = {0};
	Pixmap bitmap = XCreateBitmapFromData(display, window, empty, 1, 1);
	cursors[CURSOR_NONE] = XCreatePixmapCursor(display, bitmap, bitmap, &color, &color, 0, 0);
	
	//Load sysyems curosors
	cursors[CURSOR_ARROW]		= XCreateFontCursor(display, XC_left_ptr);
	cursors[CURSOR_HAND]		= XCreateFontCursor(display, XC_hand1);
	cursors[CURSOR_SIZENESW]	= XCreateFontCursor(display, XC_left_ptr);
	cursors[CURSOR_SIZENS]		= XCreateFontCursor(display, XC_sb_v_double_arrow);
	cursors[CURSOR_SIZENWSE]	= XCreateFontCursor(display, XC_top_left_arrow);
	cursors[CURSOR_SIZEWE]		= XCreateFontCursor(display, XC_left_ptr);
	
	//for(unsigned i = 0; i < CURSOR_LAST; i++) XFreeCursor(display, cursors[i]);
}

//=============================================================================
// Sets the fullscreen mode
//=============================================================================
void LinuxSystem::setFullscreen(bool fullscreen)
{
	if(fullscreen) {
		//XF86VidModeSwitchToMode(display, screen, &fullscreenMode);
		//XF86VidModeSetViewPort(display, screen, 0, 0);
		//XFlush(display);

		//XGrabKeyboard(display, mainWindow, True, GrabModeAsync, GrabModeAsync, CurrentTime);
		//XGrabPointer(display, mainWindow, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, mainWindow, None, CurrentTime);
		//XWarpPointer(display, None, mainWindow, 0, 0, 0, 0, 0, 0);
	} else {
		//XUngrabPointer(display, CurrentTime);
		//XUngrabKeyboard(display, CurrentTime);

		//XF86VidModeSwitchToMode(display, screen, &desktopMode);
		//XF86VidModeSetViewPort(display, screen, 0, 0);
		//XFlush(display);
	}
}

//=============================================================================
// Converts X11 keys to the engine's ones
//=============================================================================
KeyEvent::KeyCode LinuxSystem::translateKey(int keyCode)
{
	//a-z
	if(keyCode >= XK_a && keyCode <= XK_z) {
		return static_cast<KeyEvent::KeyCode>(KeyEvent::K_A + keyCode - XK_a);
	}
	if(keyCode >= XK_A && keyCode <= XK_A) {
		return static_cast<KeyEvent::KeyCode>(KeyEvent::K_A + keyCode - XK_A);
	}

	//0-9
	if (keyCode >= XK_0 && keyCode <= XK_9) {
		return static_cast<KeyEvent::KeyCode>(KeyEvent::K_0 + keyCode - XK_0);
	}

	//Mouse buttons
	switch(keyCode)
	{
	case Button1:			return KeyEvent::MK_LEFT;
	case Button2:			return KeyEvent::MK_MIDDLE;
	case Button3:			return KeyEvent::MK_RIGHT;

	case XK_space:			return KeyEvent::K_SPACE;

	case XK_ISO_Level3_Shift:	return KeyEvent::K_RALT; //IT Alt Gr

	case XK_BackSpace:		return KeyEvent::K_BACKSPACE;
	case XK_Tab:			return KeyEvent::K_TAB;
	case XK_Clear:			return KeyEvent::K_CLEAR;
	case XK_Return:			return KeyEvent::K_ENTER;
	case XK_Pause:			return KeyEvent::K_PAUSE;
	case XK_Escape:			return KeyEvent::K_ESCAPE;
	case XK_Delete:			return KeyEvent::K_DELETE;
	case XK_Home:			return KeyEvent::K_HOME;
	case XK_Left:			return KeyEvent::K_LEFT;
	case XK_Up:				return KeyEvent::K_UP;
	case XK_Right:			return KeyEvent::K_RIGHT;
	case XK_Down:			return KeyEvent::K_DOWN;
	case XK_Prior:			return KeyEvent::K_PRIOR;
	case XK_Next:			return KeyEvent::K_NEXT;
	case XK_End:			return KeyEvent::K_END;
	case XK_Insert:			return KeyEvent::K_INSERT;
	case XK_Num_Lock:		return KeyEvent::K_NUMLOCK;
	case XK_KP_Space:		return KeyEvent::K_SPACE;
	case XK_KP_Enter:		return KeyEvent::K_ENTER;
	case XK_KP_Home:		return KeyEvent::K_HOME;
	case XK_KP_Left:		return KeyEvent::K_LEFT;
	case XK_KP_Up:			return KeyEvent::K_UP;
	case XK_KP_Right:		return KeyEvent::K_RIGHT;
	case XK_KP_Down:		return KeyEvent::K_DOWN;
	case XK_KP_Prior:		return KeyEvent::K_PRIOR;
	case XK_KP_Next:		return KeyEvent::K_NEXT;
	case XK_KP_End:			return KeyEvent::K_END;
	case XK_KP_Insert:		return KeyEvent::K_INSERT;
	case XK_KP_Delete:		return KeyEvent::K_DELETE;
	case XK_KP_Multiply:	return KeyEvent::K_MULTIPLY;
	case XK_KP_Add:			return KeyEvent::K_ADD;
	case XK_KP_Subtract:	return KeyEvent::K_SUBTRACT;
	case XK_KP_Decimal:		return KeyEvent::K_DECIMAL;
	case XK_KP_Divide:		return KeyEvent::K_DIVIDE;
	case XK_Shift_L:		return KeyEvent::K_LSHIFT;
	case XK_Shift_R:		return KeyEvent::K_RSHIFT;
	case XK_Control_L:		return KeyEvent::K_LCTRL;
	case XK_Control_R:		return KeyEvent::K_RCTRL;
	case XK_Caps_Lock:		return KeyEvent::K_CAPITAL;
	case XK_Alt_L:			return KeyEvent::K_LALT;
	case XK_Alt_R:			return KeyEvent::K_RALT;
	}

	//Numpad
	if((keyCode >= XK_KP_0) && (keyCode <= XK_KP_9))
		return static_cast<KeyEvent::KeyCode>(KeyEvent::KP_0 + keyCode - XK_KP_0);

	//F1-F22
	if((keyCode >= XK_F1) && (keyCode <= XK_F22))
		return static_cast<KeyEvent::KeyCode>(KeyEvent::K_F1 + keyCode - XK_F1);


	return KeyEvent::K_UNKNOW;
}

//=============================================================================
// Sends mouse events
//=============================================================================
void LinuxSystem::updateMouse()
{
    if(engine->isPaused()) return;

    int tmpInt;
    Window tmpWnd;
    unsigned tmpUInt;

    int mouseX, mouseY;
    XQueryPointer(display, rootWindow, &tmpWnd, &tmpWnd, &mouseX, &mouseY, &tmpInt, &tmpInt, &tmpUInt);

    XWindowAttributes windowAttr;
    XGetWindowAttributes(display, mainWindow, &windowAttr);

    int windowX, windowY;
    XTranslateCoordinates(display, mainWindow, rootWindow, windowAttr.x, windowAttr.y, &windowX,
                          &windowY, &tmpWnd);

    if(mouseLocked) {
        int windowCenterX = windowX + windowAttr.width / 2;
		int windowCenterY = windowY + windowAttr.height / 2;

		//First time, only reset the mouse position
		if(!mouseResetted) {
            XWarpPointer(display, None, rootWindow, 0, 0, 0, 0, windowCenterX, windowCenterY);
			mouseResetted = true;
			return;
		}

		//The mouse didn't move
		if((mouseX == windowCenterX) && (mouseY == windowCenterY)) {
			return;
		}

		//Reset mouse position
		XWarpPointer(display, None, rootWindow, 0, 0, 0, 0, windowCenterX, windowCenterY);

		mouseX -= windowCenterX;
		mouseY -= windowCenterY;
    } else {
        mouseX -= windowX;
		mouseY -= windowY;
    }

    MouseEvent mouseEvent(mouseX, mouseY);
    mouseEventSender.sendEvent(mouseEvent);
}
