//#include "MsgEvent.h"
#include "Container.h"

#include "memdbg.h"

using myengine::Vec2i;
using myengine::gui::Container;
using myengine::gui::MsgEvent;


//=============================================================================
// Constructor
//=============================================================================
Container::Container(Container *parent): Object(parent)
{
	focused = undermouse = nullptr;
}

//=============================================================================
// Destructor
//=============================================================================
Container::~Container()
{
	removeObjects();
}

//=============================================================================
// Called when the user press a key
//=============================================================================
void Container::onChar(char c)
{
	if(focused) focused->onChar(c);
}

//=============================================================================
// Calls the function onDraw for all windows
//=============================================================================
void Container::onDraw()
{
	const List<Object*>::Node *node = objects.getTail();
	while(node) {
		node->element->onDraw();
		node = node->prev;
	}
}

//=============================================================================
// Called when the mouse is pressed
//=============================================================================
void Container::onMousePressed()
{
	if(undermouse) {
		if(undermouse != focused) setFocused(undermouse);
		undermouse->onMousePressed();
	} else {
		setFocused(nullptr);
	}
}

//=============================================================================
// Called when the mouse is released
//=============================================================================
void Container::onMouseReleased()
{
	if(undermouse) undermouse->onMouseReleased();
}

//=============================================================================
// Called when the mouse moves
//=============================================================================
void Container::onMouseMove(const Vec2i &position)
{
	//Fix for moving objects
	if(undermouse) {
		undermouse->onMouseMove(position - undermouse->getPosition());
	}

	const List<Object*>::Node *node = objects.getHead();
	while(node) {
		Object *object = node->element;
		if(object->contains(position)) {
			if(object != undermouse) setUndermouse(object);
			if(object) object->onMouseMove(position - object->getPosition());
			break;
		}
		node = node->next;
	}
	if(!node) setUndermouse(nullptr);
}

//=============================================================================
// Adds the specified window
//=============================================================================
void Container::add(Object *object)
{
	objects.addFirst(object);
	object->onCreate();
	setFocused(object);
}

//=============================================================================
// Removes the specified window
//=============================================================================
void Container::remove(Object *object)
{
	if(object == focused) focused = nullptr;
	if(object == undermouse) undermouse = nullptr;

	object->onDestroy();
	objects.remove(object);
	delete object;
}

//=============================================================================
// Removes all windows
//=============================================================================
void Container::removeObjects()
{
	while(!objects.isEmpty()) {
		remove(objects.getFirst());
	}
}

//=============================================================================
// Sets the specified object as focused
//=============================================================================
void Container::setFocused(Object *object)
{
	if(focused) focused->onLoseFocus();
	focused = object;
	if(focused) {
		focused->onFocus();
		objects.remove(focused);
		objects.addFirst(focused);
	}
}

//=============================================================================
// Called when a children send a message
//=============================================================================
void Container::onEvent(MsgEvent &msgEvent)
{
}

//=============================================================================
// Called every 33ms (30 fps)
//=============================================================================
/*void Window::onTick()
{
}*/

//=============================================================================
// Sets the specified object as undermouse
//=============================================================================
void Container::setUndermouse(Object *object)
{
	if(undermouse) undermouse->onMouseOut();
	undermouse = object;
	if(undermouse) undermouse->onMouseOver();
}