#pragma once
#ifndef __WIN_WNDPROC_H__
#define __WIN_WNDPROC_H__


#include "WindowsHeader.h"


namespace myengine {

	extern bool mouseResetted;	//True if the mouse has been positioned at center

	LRESULT CALLBACK mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

}


#endif //!__WIN_WNDPROC_H__