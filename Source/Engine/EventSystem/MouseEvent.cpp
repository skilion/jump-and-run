#include "Sender.h"
#include "MouseEvent.h"

#include "Math/Vector.h"

#include "memdbg.h"

using namespace myengine;


Sender<MouseEvent> myengine::mouseEventSender;


//=============================================================================
// Constructor
//=============================================================================
MouseEvent::MouseEvent(int x, int y)
{
	this->x = x;
	this->y = y;
}

//=============================================================================
// Destructor
//=============================================================================
//MouseEvent::~MouseEvent() {}

//=============================================================================
// Returns the mouse position
//=============================================================================
void MouseEvent::get(int &x, int &y) const
{
	x = this->x;
	y = this->y;
}

void MouseEvent::get(Vec2i &v) const
{
	v.set(x, y);
}

//=============================================================================
// Returns the X coordinate
//=============================================================================
int MouseEvent::getX() const
{
	return x;
}

//=============================================================================
// Returns the X coordinate
//=============================================================================
int MouseEvent::getY() const
{
	return y;
}