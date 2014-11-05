#include "Platform.h"
#include "WindowsThread.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Create a thread
//=============================================================================
Thread *myengine::createThread(Runnable *runnable, bool autoDelete)
{
	ASSERT(runnable);
	return new WindowsThread(runnable, autoDelete);
}

//=============================================================================
// Waits until the time-out interval elapses
//=============================================================================
void Thread::sleep(unsigned milliseconds)
{
	Sleep(milliseconds);
}

//=============================================================================
// Switch to another thread
//=============================================================================
void Thread::yield()
{
	SwitchToThread();
}

//=============================================================================
// Constructor
//=============================================================================
WindowsThread::WindowsThread(Runnable *runnable, bool autoDelete): Thread(runnable, autoDelete)
{
	hThread = 0;
}

//=============================================================================
// Destructor
//=============================================================================
//WindowsThread::~WindowsThread() {}

//=============================================================================
// Returns true if the thread is active
//=============================================================================
bool WindowsThread::isActive()
{
	if(hThread/* && !GetExitCodeThread(hThread, 0) && GetLastError() == STILL_ACTIVE*/) {
		return true;
	}
	return false;
}

//=============================================================================
// Starts the thread
//=============================================================================
void WindowsThread::start()
{
	if(!isActive()) {
		hThread = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entryPoint), this, 0, 0);
	}
}

//=============================================================================
// Stops the thread
//=============================================================================
void WindowsThread::kill()
{
	TerminateThread(hThread, 1);
	CloseHandle(hThread); //todo: check error
	hThread = 0;
}

//=============================================================================
// Waits until the thread exits
//=============================================================================
void WindowsThread::join()
{
	WaitForSingleObject(hThread, INFINITE);
}

//=============================================================================
// Waits until the thread exits
//=============================================================================
void WindowsThread::join(unsigned milliseconds)
{
	WaitForSingleObject(hThread, milliseconds);
}

//=============================================================================
// Entry point of the thread
//=============================================================================
void WindowsThread::closeHandle()
{
	CloseHandle(hThread);
	hThread = 0;
}

//=============================================================================
// Entry point of the thread
//=============================================================================
void WindowsThread::entryPoint(WindowsThread *thread)
{
	thread->runnable->run();
	thread->closeHandle();

	if(thread->autoDelete) delete thread;

	ExitThread(0);
}
