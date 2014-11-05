#include "Memory.h"
#include "myString.h"
#include "Platform.h"
#include "WindowsLog.h"

#include "System/WindowsHeader.h"

#include "memdbg.h"

using namespace myengine;


//Constants for the log window
const char * const	LOG_WND_CLASS		= "LogWindowClass";
const char * const	LOG_WND_TITLE		= "Log Window";
const int			LOG_WND_WIDTH		= 640;
const int			LOG_WND_HEIGHT		= 480;
const unsigned		LOG_WND_MAX_CHARS	= 0xFFFF;


//=============================================================================
// Create a Log class
//=============================================================================
Log *Log::create()
{
	return new WindowsLog;
}


//=============================================================================
// Constructor
//=============================================================================
WindowsLog::WindowsLog()
{
	hWnd		= 0;
	hwndEdit	= 0;
	hFont		= 0;
	charCount	= 0;

	createWindow();
}

//=============================================================================
// Destructor
//=============================================================================
WindowsLog::~WindowsLog()
{
	destroyWindow();
}

//=============================================================================
// Prints the specified string
//=============================================================================
void WindowsLog::print(const char *string)
{
	unsigned len = String::strlen(string);//, newLineCount = 0;

	charCount += len + 2;
	/*for(unsigned i = 0; i < len; i++)
		if(string[i] == '\n') newLineCount++;
	totalChars += newLineCount;

	char *winString = new char[strChars + newLineCount + 1];
	for(unsigned i = 0, j = 0; i <= strChars; i++, j++) {
		char tmp = string[i];
		if(tmp == '\n') {
			winString[j] = '\r';
			winString[++j] = '\n';
		} else winString[j] = tmp;
	}*/
	
	//Select everything
	//Edit_SetSel(hwndOutputEdit, 0, -1);
	SendMessage(hwndEdit, EM_SETSEL, 0, -1);

	if(charCount > LOG_WND_MAX_CHARS) charCount = len + 2;	//Restart char count
	else SendMessage(hwndEdit, EM_SETSEL, -1, 0);			//Deselect everything
	
	//Replace selected text
	SendMessage(hwndEdit, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(string));	
	SendMessage(hwndEdit, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(NEWLINE));	

	//delete [] winString;

	Log::print(string);
}

//=============================================================================
// Creates the window
//=============================================================================
void WindowsLog::createWindow()
{
	WNDCLASSEX wc;
	Memory::memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.lpfnWndProc   = DefWindowProc;
	wc.hInstance     = GetModuleHandle(0);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = HBRUSH(COLOR_BTNFACE + 1);
	wc.lpszClassName = LOG_WND_CLASS;

	if(!RegisterClassEx(&wc)) return;
	
	DWORD style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE;

	RECT rect;
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = LOG_WND_WIDTH;
	rect.bottom = LOG_WND_HEIGHT;
	AdjustWindowRect(&rect, style, 0);

	int x = 10;//(GetSystemMetrics(SM_CXSCREEN) - LOG_WND_WIDTH) / 2;
	int y = 10;//(GetSystemMetrics(SM_CYSCREEN) - LOG_WND_HEIGHT) / 2;
	
	hWnd = CreateWindowEx(0,
		LOG_WND_CLASS,
		LOG_WND_TITLE,
		style, x, y,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0, 0, 0, 0);
	if(!hWnd) return;
	
	int fontHeight = -MulDiv(10,
		GetDeviceCaps(GetDC(hWnd), LOGPIXELSY),
		72);

	hFont = CreateFont(fontHeight,
		0, 0, 0, FW_NORMAL,
		0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN,
		"Courier New");
	if(!hFont) return;

	hwndEdit = CreateWindowEx(0, "EDIT", 0,
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		WS_BORDER, 6, 6, LOG_WND_WIDTH - 12, LOG_WND_HEIGHT - 12,
		hWnd, 0, GetModuleHandle(0), 0);
	if(!hwndEdit) return;

	SendMessage(hwndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 0);
	SendMessage(hwndEdit, EM_LIMITTEXT, LOG_WND_MAX_CHARS, 0);

	UpdateWindow(hWnd);
}

//=============================================================================
// Close the window
//=============================================================================
void WindowsLog::destroyWindow()
{
	DestroyWindow(hwndEdit);
	DeleteObject(hFont);
	DestroyWindow(hWnd);
	UnregisterClass(LOG_WND_CLASS, GetModuleHandle(0));
}