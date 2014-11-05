#include "Sender.h"
#include "KeyEvent.h"

#include "memdbg.h"

using namespace myengine;


Sender<KeyEvent> myengine::keyEventSender;


//=============================================================================
// Constructor
//=============================================================================
KeyEvent::KeyEvent(KeyCode keyCode, KeyState keyState)
{
	this->keyCode = keyCode;
	this->keyState = keyState;
}

//=============================================================================
// Destructor
//=============================================================================
//KeyEvent::~KeyEvent() {}

//=============================================================================
// Returns the key code
//=============================================================================
KeyEvent::KeyCode KeyEvent::getKeyCode() const
{
	return keyCode;
}

//=============================================================================
// Returns the key state
//=============================================================================
KeyEvent::KeyState KeyEvent::getKeyState() const
{
	return keyState;
}