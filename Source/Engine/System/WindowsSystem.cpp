#include "Engine.h"
#include "myString.h"
#include "WindowsSystem.h"
#include "WindowsWndProc.h"

#include "EventSystem/MouseEvent.h"
#include "EventSystem/Sender.h"

#include "memdbg.h"

using namespace myengine;


WindowsSystem	*myengine::windowsSystem = 0;


//=============================================================================
// Creates the System class
//=============================================================================
System *System::create()
{
	if(!windowsSystem) {
		windowsSystem = new WindowsSystem;
	}

	return windowsSystem;
}


//=============================================================================
// Constructor
//=============================================================================
WindowsSystem::WindowsSystem()
{
	QueryPerformanceCounter(&start);
	QueryPerformanceFrequency(&frequency);
	frequency.QuadPart /= 1000;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	cursors[CURSOR_ARROW]		= (HCURSOR)LoadImage(0, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	cursors[CURSOR_HAND]		= (HCURSOR)LoadImage(0, MAKEINTRESOURCE(OCR_HAND), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	cursors[CURSOR_SIZENESW]	= (HCURSOR)LoadImage(0, MAKEINTRESOURCE(OCR_SIZENESW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	cursors[CURSOR_NONE]		= (HCURSOR)LoadImage(0, MAKEINTRESOURCE(OCR_SIZENS), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	cursors[CURSOR_SIZEWE]		= (HCURSOR)LoadImage(0, MAKEINTRESOURCE(OCR_SIZENWSE), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	cursors[CURSOR_SIZEWE]		= (HCURSOR)LoadImage(0, MAKEINTRESOURCE(OCR_SIZEWE), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	mouseLocked = false;
	mouseResetted = false;
}

//=============================================================================
// Destructor
//=============================================================================
WindowsSystem::~WindowsSystem()
{
	WSACleanup();
}

//=============================================================================
// Returns the contenent of the clipboard
//=============================================================================
const char *WindowsSystem::getClipboardText()
{
	if(!IsClipboardFormatAvailable(CF_TEXT)) return 0;
	if(!OpenClipboard(0)) return 0;

	HANDLE hClipboard = GetClipboardData(CF_TEXT);
	if(!hClipboard) return 0;

	String str(static_cast<const char*>(GlobalLock(hClipboard)));

	GlobalUnlock(hClipboard);
	CloseClipboard();

	return str.getString();
}

//=============================================================================
// Returns the amount of free space on the disk in bytes
//=============================================================================
unsigned WindowsSystem::getFreeSpaceMegaBytes()
{
	ULARGE_INTEGER freeSpace;
	GetDiskFreeSpaceEx(0, &freeSpace, 0, 0);

	freeSpace.QuadPart /= 1024 * 1024;

	if(freeSpace.QuadPart > 0xFFFFFFFF) return 0xFFFFFFFF;
	return static_cast<unsigned>(freeSpace.QuadPart);
}

//=============================================================================
// Returns the time in milliseconds since the program has started
//=============================================================================
unsigned WindowsSystem::getMilliseconds()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	counter.QuadPart -= start.QuadPart;

	return static_cast<unsigned>(counter.QuadPart / frequency.QuadPart);
}

//=============================================================================
// Returns some desktop information
//=============================================================================
void WindowsSystem::getDesktopInfo(int &width, int &height, int &colors)
{
	HDC hDC	= GetDC(0);
	width	= GetDeviceCaps(hDC, HORZRES);
	height	= GetDeviceCaps(hDC, VERTRES);
	colors	= GetDeviceCaps(hDC, BITSPIXEL);
	ReleaseDC(0, hDC);
}

//=============================================================================
// Handles Windows events
//=============================================================================
void WindowsSystem::handleEvents()
{
	MSG msg;
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT) {
			engine->exit();
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	updateMouse();
}

//=============================================================================
// Sets the contenent of the clipboard
//=============================================================================
void WindowsSystem::setClipboardText(const char *text)
{
	if(!OpenClipboard(0)) return;
	EmptyClipboard();

	HGLOBAL hClipboard = GlobalAlloc(GMEM_MOVEABLE, String::strlen(text) + 1);

	if(!hClipboard) {
		CloseClipboard();
		return;
	}

	char *dest = static_cast<char*>(GlobalLock(hClipboard));
	String::strcpy(dest, text);

	GlobalUnlock(hClipboard);
	SetClipboardData(CF_TEXT, hClipboard);
	CloseClipboard();
}

//=============================================================================
// Sets the mouse cursor
//=============================================================================
void WindowsSystem::setCursor(WindowsSystem::Cursor cursor)
{
	switch(cursor)
	{
	case CURSOR_NONE:
		ShowCursor(0);
		break;
		
	default:
		ShowCursor(1);
		SetCursor(cursors[cursor]);
	}
}

//=============================================================================
// Sest if the mouse is locked at center
//=============================================================================
void WindowsSystem::setMouseLocked(bool mouseLocked)
{
	this->mouseLocked = mouseLocked;
	mouseResetted = false;
}

//=============================================================================
// Alert the user
//=============================================================================
void WindowsSystem::messageBox(const char *text, const char *caption)
{
	MessageBox(0, text, caption, MB_ICONEXCLAMATION);
}

//=============================================================================
// Pauses the program till user quit
//=============================================================================
void WindowsSystem::pauseTillExit()
{
	MSG msg;
	while(true) {
		if(!GetMessage(&msg, 0, 0, 0)) {
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//=============================================================================
// Waits until the time-out interval elapses
//=============================================================================
void WindowsSystem::sleep(unsigned milliseconds)
{
	Sleep(milliseconds);
}

//=============================================================================
// Sets the main window
//=============================================================================
void WindowsSystem::setMainWindow(HWND hWnd)
{
	mainWindow = hWnd;
}

//=============================================================================
// Sends mouse events
//=============================================================================
void WindowsSystem::updateMouse()
{
	if(engine->isPaused()) return;

	RECT rect;
	GetClientRect(mainWindow, &rect);
	POINT min = {0, 0};
	POINT max = {rect.right, rect.bottom};
	ClientToScreen(mainWindow, &min);
	ClientToScreen(mainWindow, &max);

	POINT mouse;
	GetCursorPos(&mouse);

	if(mouseLocked) {
		int windowCenterX = min.x + rect.right  / 2;
		int windowCenterY = min.y + rect.bottom / 2;

		//First time, only reset the mouse position
		if(!mouseResetted) {
			mouseResetted = true;
			SetCursorPos(windowCenterX, windowCenterY);
			return;
		}

		//The mouse didn't move
		if((mouse.x == windowCenterX) && (mouse.y == windowCenterY)) {
			return;
		}

		//Reset mouse position
		SetCursorPos(windowCenterX, windowCenterY);

		mouse.x -= windowCenterX;
		mouse.y -= windowCenterY;
	} else {
		if(mouse.x < min.x || mouse.y < min.y) return;
		if(mouse.x > max.x || mouse.y > max.y) return;

		//todo
		//The mouse didn't move
		/*if((mouse.x == oldX) && (mouse.y == oldY)) {
			return;
		}*/
		mouse.x -= min.x;
		mouse.y -= min.y;
	}

	mouseEventSender.sendEvent(MouseEvent(mouse.x, mouse.y));
}
