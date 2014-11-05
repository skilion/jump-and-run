#include <Memory.h>

#include "Log.h"
#include "Engine.h"
#include "WindowsWndProc.h"

#include "EventSystem/CharEvent.h"
#include "EventSystem/KeyEvent.h"
#include "EventSystem/Sender.h"
#include "Render/Render.h"

#include "memdbg.h"

using namespace myengine;


bool myengine::mouseResetted;

//=============================================================================
// Converts Windows Virtual Keys to the engine's ones
//=============================================================================
KeyEvent::KeyCode translateKey(unsigned sysKeyCode)
{
	//a-z
	if((sysKeyCode >= 0x41) && (sysKeyCode <= 0x5A))
		return static_cast<KeyEvent::KeyCode>(KeyEvent::K_A + sysKeyCode - 0x41);

	//0-9
	if((sysKeyCode >= 0x30) && (sysKeyCode <= 0x39))
		return static_cast<KeyEvent::KeyCode>(KeyEvent::K_0 + sysKeyCode - 0x30);

	switch(sysKeyCode)
	{
	/*case VK_LBUTTON:
		BREAKPOINT;
		return MK_LEFT;
	case VK_RBUTTON:
		return MK_RIGHT;
	case VK_MBUTTON:
		return MK_MIDDLE;*/
	case VK_BACK:		return KeyEvent::K_BACKSPACE;
	case VK_TAB:		return KeyEvent::K_TAB;
	case VK_CLEAR:		return KeyEvent::K_CLEAR;
	case VK_RETURN:		return KeyEvent::K_ENTER;
	case VK_PAUSE:		return KeyEvent::K_PAUSE;
	case VK_CAPITAL:	return KeyEvent::K_CAPITAL;
	case VK_ESCAPE:		return KeyEvent::K_ESCAPE;
	case VK_SPACE:		return KeyEvent::K_SPACE;
	case VK_PRIOR:		return KeyEvent::K_PRIOR;
	case VK_NEXT:		return KeyEvent::K_NEXT;
	case VK_END:		return KeyEvent::K_END;
	case VK_HOME:		return KeyEvent::K_HOME;
	case VK_LEFT:		return KeyEvent::K_LEFT;
	case VK_UP:			return KeyEvent::K_UP;
	case VK_RIGHT:		return KeyEvent::K_RIGHT;
	case VK_DOWN:		return KeyEvent::K_DOWN;
	case VK_INSERT:		return KeyEvent::K_INSERT;
	case VK_DELETE:		return KeyEvent::K_DELETE;
	case VK_MULTIPLY:	return KeyEvent::K_MULTIPLY;
	case VK_ADD:		return KeyEvent::K_ADD;
	//case VK_SEPARATOR:	return KeyEvent::KP_SEPARATOR;
	case VK_SUBTRACT:	return KeyEvent::K_SUBTRACT;
	case VK_DECIMAL:	return KeyEvent::K_DECIMAL;
	case VK_DIVIDE:		return KeyEvent::K_DIVIDE;
	case VK_NUMLOCK:	return KeyEvent::K_NUMLOCK;
	case VK_LSHIFT:		return KeyEvent::K_LSHIFT;
	case VK_RSHIFT:		return KeyEvent::K_RSHIFT;
	case VK_LCONTROL:	return KeyEvent::K_LCTRL;
	case VK_RCONTROL:	return KeyEvent::K_RCTRL;
	case VK_LMENU:		return KeyEvent::K_LALT;
	case VK_RMENU:		return KeyEvent::K_RALT;
	case VK_OEM_1:		return KeyEvent::K_OEM_1;
	case VK_OEM_PLUS:	return KeyEvent::K_OEM_PLUS;
	case VK_OEM_COMMA:	return KeyEvent::K_OEM_COMMA;
	case VK_OEM_MINUS:	return KeyEvent::K_OEM_MINUS;
	case VK_OEM_PERIOD:	return KeyEvent::K_OEM_PERIOD;
	case VK_OEM_2:		return KeyEvent::K_OEM_2;
	case VK_OEM_3:		return KeyEvent::K_OEM_3;
	case VK_OEM_4:		return KeyEvent::K_OEM_4;
	case VK_OEM_5:		return KeyEvent::K_OEM_5;
	case VK_OEM_6:		return KeyEvent::K_OEM_6;
	case VK_OEM_7:		return KeyEvent::K_OEM_7;
	//case VK_OEM_8:		return KeyEvent::K_OEM_8;
	case VK_OEM_102:	return KeyEvent::K_OEM_102;
	}

	//Numpad
	if((sysKeyCode >= VK_NUMPAD0) && (sysKeyCode <= VK_NUMPAD9))
		return static_cast<KeyEvent::KeyCode>(KeyEvent::KP_0 + sysKeyCode - VK_NUMPAD0);

	//F1-F22
	if((sysKeyCode >= VK_F1) && (sysKeyCode <= VK_F22))
		return static_cast<KeyEvent::KeyCode>(KeyEvent::K_F1 + sysKeyCode - VK_F1);

	return KeyEvent::K_UNKNOW;
}

//=============================================================================
// Main window procedure
//=============================================================================
LRESULT CALLBACK myengine::mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//static int windowCenterX, windowCenterY;

	switch(message)
	{
	case WM_ACTIVATE:
		if(LOWORD(wParam) != WA_INACTIVE) {
			if(render->isFullscreen()) {
				DEVMODE dm;
				Memory::memset(&dm, 0, sizeof(DEVMODE));
				dm.dmSize       = sizeof(DEVMODE);
				dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
				dm.dmPelsWidth  = render->getResolution().getX();
				dm.dmPelsHeight = render->getResolution().getY();
				dm.dmBitsPerPel = render->getColors();

				if(ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
					 log->print("Can't set the requested fullscreen mode");
				}

				ShowWindow(hWnd, SW_RESTORE);
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
			}
			engine->setPause(false);
			mouseResetted = false;
		} else {
			if(render->isFullscreen()) {
				ChangeDisplaySettings(0, 0);
				ShowWindow(hWnd, SW_MINIMIZE);
			}
			engine->setPause(true);
		}
		return 0;

	case WM_CHAR:
		if(wParam < 128) {
			charEventSender.sendEvent(CharEvent(static_cast<char>(wParam)));
			return 0;
		}
		break;

	case WM_CLOSE:
		/*if(MessageBox(hWnd, "Do you really want to close ?", "WIN_MAIN_WND_TITLE", MB_YESNO | MB_ICONWARNING) == IDNO) {
			return 0;
		}*/
		PostQuitMessage(0);
		break;

	case WM_PAINT: //todo
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			TextOut(hdc, 6, 6, "Loading...", 10);
			EndPaint(hWnd, &ps);
			return 0;
		}

	//Keyboard
	case WM_SYSKEYDOWN:
		if((wParam == VK_F4)) { //Check for Alt+F4
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_SYSKEYUP:
		{
			bool isExtended = (lParam & (1 << 24)) != 0;/*KF_EXTENDED*/
			switch(wParam)
			{
			case VK_SHIFT:
				wParam = ((lParam & 0xFF0000) == 0x360000) ? VK_RSHIFT : VK_LSHIFT;
				break;
			case VK_CONTROL:
				wParam = isExtended ? VK_RCONTROL : VK_LCONTROL;
				break;
			case VK_MENU:
				wParam = isExtended ? VK_RMENU : VK_LMENU;
				break;
			};
		}

		if(lParam & (1 << 31))  {					//bit 31 = transition state
			if(lParam & (1 << 30)) {				//bit 30 = previous key state
				keyEventSender.sendEvent(KeyEvent(translateKey(wParam), KeyEvent::KEY_RELEASE));
			}
		} else {
			if(!(lParam & (1 << 30))) {				//bit 30 = previous key state
				keyEventSender.sendEvent(KeyEvent(translateKey(wParam), KeyEvent::KEY_PRESS));
			}
		}
		return 0;

	//Mouse buttons
	case WM_LBUTTONDOWN:
		keyEventSender.sendEvent(KeyEvent(KeyEvent::MK_LEFT, KeyEvent::KEY_PRESS));
		return 0;
	case WM_LBUTTONUP:
		keyEventSender.sendEvent(KeyEvent(KeyEvent::MK_LEFT, KeyEvent::KEY_RELEASE));
		return 0;
	case WM_MBUTTONDOWN:
		keyEventSender.sendEvent(KeyEvent(KeyEvent::MK_MIDDLE, KeyEvent::KEY_PRESS));
		return 0;
	case WM_MBUTTONUP:
		keyEventSender.sendEvent(KeyEvent(KeyEvent::MK_MIDDLE, KeyEvent::KEY_RELEASE));
		return 0;
	case WM_RBUTTONDOWN:
		keyEventSender.sendEvent(KeyEvent(KeyEvent::MK_RIGHT, KeyEvent::KEY_PRESS));
		return 0;
	case WM_RBUTTONUP:
		keyEventSender.sendEvent(KeyEvent(KeyEvent::MK_RIGHT, KeyEvent::KEY_RELEASE));
		return 0;

	//Prevent to start the screensaver and to turn off the monitor
	case WM_SYSCOMMAND:
		switch(LOWORD(wParam)) {
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
