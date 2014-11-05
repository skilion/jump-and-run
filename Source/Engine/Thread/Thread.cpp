#include "Thread.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Thread::Thread(Runnable *runnable, bool autoDelete)
{
	this->runnable = runnable;
	this->autoDelete = autoDelete;
}

//=============================================================================
// Destructor
//=============================================================================
Thread::~Thread()
{
	delete runnable;
}