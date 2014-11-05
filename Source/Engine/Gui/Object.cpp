#include "Object.h"
#include "Container.h"

#include "memdbg.h"

using namespace myengine::gui;
using myengine::Vec2i;


//=============================================================================
// Constructor
//=============================================================================
Object::Object(Container *parent): position(0, 0), size(10, 10)
{
	this->parent = parent;
	if(parent) parent->add(this);
}

//=============================================================================
// Destructor
//=============================================================================
Object::~Object()
{
	//if(parent) parent->remove(this);
}

//=============================================================================
// Returns true if the object contains the specified point
//=============================================================================
bool Object::contains(const Vec2i &point) const
{
	if(point.getX() < position.getX()) return false;
	if(point.getY() < position.getY()) return false;
	if(point.getX() >= (position.getX() + size.getX())) return false;
	if(point.getY() >= (position.getY() + size.getY())) return false;
	return true;
}

//=============================================================================
// Sets the size of the object
//=============================================================================
void Object::setSize(const Vec2i &size)
{
	this->size = size;
	onChangeSize();
}

//=============================================================================
// Called when the user press a key
//=============================================================================
void Object::onChar(char c)
{
}

//=============================================================================
// Called when the object changes size
//=============================================================================
void Object::onChangeSize()
{
}

//=============================================================================
// Called when the element is registered
//=============================================================================
void Object::onCreate()
{
}

//=============================================================================
// Called when the element is unregistered
//=============================================================================
void Object::onDestroy()
{
}

//=============================================================================
// Draws the element
//=============================================================================
void Object::onDraw()
{
}

//=============================================================================
// Called when user selects the element
//=============================================================================
void Object::onFocus()
{
}

//=============================================================================
// Called when user selects another panel
//=============================================================================
void Object::onLoseFocus()
{
}

//=============================================================================
// Called when the cursor enters in the element
//=============================================================================
void Object::onMouseOver()
{
}

//=============================================================================
// Called when the cursor lefts the element
//=============================================================================
void Object::onMouseOut()
{
}

//=============================================================================
// Called when the mouse is pressed on the element
//=============================================================================
void Object::onMousePressed()
{
}

//=============================================================================
// Called when the mouse is released on the element
//=============================================================================
void Object::onMouseReleased()
{
}

//=============================================================================
// Called when the mouse moves on the element
//=============================================================================
void Object::onMouseMove(const Vec2i &position)
{
}
