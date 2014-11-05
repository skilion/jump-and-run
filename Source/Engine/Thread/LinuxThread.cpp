#include <sched.h>
#include <signal.h>
#include <unistd.h>

#include "Platform.h"
#include "LinuxThread.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Create a thread
//=============================================================================
Thread *myengine::createThread(Runnable *runnable, bool autoDelete)
{
	ASSERT(runnable);
	return new LinuxThread(runnable, autoDelete);
}

//=============================================================================
// Waits until the time-out interval elapses
//=============================================================================
void Thread::sleep(unsigned milliseconds)
{
	usleep(milliseconds * 1000); //nanosleep ?
}

//=============================================================================
// Switch to another thread
//=============================================================================
void Thread::yield()
{
	sched_yield();
}

//=============================================================================
// Constructor
//=============================================================================
LinuxThread::LinuxThread(Runnable *runnable, bool autoDelete): Thread(runnable, autoDelete)
{
	//hThread = 0;
}

//=============================================================================
// Destructor
//=============================================================================
//LinuxThread::~LinuxThread() {}

//=============================================================================
// Returns true if the thread is active
//=============================================================================
bool LinuxThread::isActive()
{
	if(0/* && !GetExitCodeThread(hThread, 0) && GetLastError() == STILL_ACTIVE*/) {
		return true;
	}
	return false;
}

//=============================================================================
// Starts the thread
//=============================================================================
void LinuxThread::start()
{
	if(!isActive()) {
		pthread_create(&pThread, 0, reinterpret_cast<void*(*)(void*)>(entryPoint), this);
	}
}

//=============================================================================
// Stops the thread
//=============================================================================
void LinuxThread::kill()
{
	pthread_kill(pThread, SI_USER);
}

//=============================================================================
// Waits until the thread exits for the specified milliseconds
//=============================================================================
void LinuxThread::join()
{
	pthread_join(pThread, 0);
}

//=============================================================================
// Waits until the thread exits for the specified milliseconds
//=============================================================================
void LinuxThread::join(unsigned milliseconds)
{
	pthread_join(pThread, 0); //todo !
}

//=============================================================================
// Entry point of the thread
//=============================================================================
void LinuxThread::entryPoint(LinuxThread *thread)
{
	thread->runnable->run();

	//thread->closeHandle();

	if(thread->autoDelete) delete thread;

	pthread_exit(0);
}
