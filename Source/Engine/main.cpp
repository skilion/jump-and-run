#include "Engine.h"

#include "memdbg.h"

using namespace myengine;


int main(int argc, char *argv[]);


#ifdef WIN32
	#include "myString.h"
	#include "System/WindowsHeader.h"

	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		/*#if defined(_DEBUG) && defined(DONT_USE_MEM_MGR)
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		#endif*/

		int argc = 1 + String::countChar(lpCmdLine, ' '); //todo: bugged
		char **argv = new char*[argc];

		argv[0] = new char[/*MAX_STRING*/100];
		GetModuleFileName(0, argv[0], /*MAX_STRING*/100);

		//Parse arguments
		if(argc > 1) {
			char *token = String::strtok(lpCmdLine, " ");
			for(int i = 1; i < argc; i++) {
				argv[i] = token;
				token = String::strtok(0, " ");
			}
		}

		int ret = main(argc, argv);
		
		delete [] *argv;
		argv[0] = 0;
		delete [] argv;

		return ret;
	}
#endif

//=============================================================================
// Application entry point
//=============================================================================
int main(int argc, char *argv[])
{
	return Engine::main();
}