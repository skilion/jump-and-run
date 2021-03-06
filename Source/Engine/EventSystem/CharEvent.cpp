#include "Sender.h"
#include "CharEvent.h"

#include "memdbg.h"

using namespace myengine;


Sender<CharEvent> myengine::charEventSender;


//=============================================================================
// Constructor
//=============================================================================
CharEvent::CharEvent(char c)
{
	this->c = c;
}

//=============================================================================
// Destructor
//=============================================================================
//CharEvent::~CharEvent() {}

//=============================================================================
// Return the character
//=============================================================================
char CharEvent::getChar() const
{
	return c;
}