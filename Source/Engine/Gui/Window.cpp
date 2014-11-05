#include "Render/Render.h"

#include "Gui.h"
#include "System.h"
#include "Window.h"

#include "memdbg.h"

using namespace myengine;
using namespace myengine::gui;


//=============================================================================
// Constructor
//=============================================================================
Window::Window(Container *parent, const String &title): Container(parent)
{
	this->title = title;
	focused = true;
	moving = false;

	setPosition(render->getResolution() / 4);
	setSize(guiStyle->dimensions.windowSize);
}

//=============================================================================
// Destructor
//=============================================================================
//Window::~Window() {}

//=============================================================================
// Called when the user press a button
//=============================================================================
/*void Window::onChar(char c)
{
	sendChar(c);
}*/

//=============================================================================
// Called when the object changes size
//=============================================================================
void Window::onChangeSize()
{
	int bordersSize = guiStyle->dimensions.windowBorder * 2;
	Vec2i adjust(bordersSize, bordersSize + guiStyle->dimensions.captionHeight);
	size += adjust;
}

//=============================================================================
// Called when the window have to be drawn
//=============================================================================
void Window::onDraw()
{
	Vec2i contentAdjust = Vec2i(guiStyle->dimensions.windowBorder, guiStyle->dimensions.windowBorder + guiStyle->dimensions.captionHeight);
	
	//Borders
	/*if(focused) render->setDrawColor(guiStyle->colors.activeWindowBorder);
	else render->setDrawColor(guiStyle->colors.inactiveWindowBorder);*/
	if(focused) render->setDrawColor(guiStyle->colors.activeCaption);
	else render->setDrawColor(guiStyle->colors.inactiveCaption);
	render->drawFilledRect(getPosition(), getSize());

	//Window background
	if(focused) render->setDrawColor(guiStyle->colors.activeWindow);
	else render->setDrawColor(guiStyle->colors.inactiveWindow);
	Vec2i borderSize(guiStyle->dimensions.windowBorder, guiStyle->dimensions.windowBorder);
	render->drawFilledRect(getPosition() + borderSize, getSize() - borderSize * 2);
	
	//Caption
	if(focused) render->setDrawColor(guiStyle->colors.activeCaption);
	else render->setDrawColor(guiStyle->colors.inactiveCaption);
	render->drawFilledRect(getPosition(), Vec2i(getSize().getX(), contentAdjust.getY()));

	//Caption text
	Vec2i captionPosition = getPosition() + Vec2i(10, (guiStyle->dimensions.captionHeight - guiStyle->captionFont->getHeight()) / 2);
	if(focused) render->setDrawColor(guiStyle->colors.activeCaptionText);
	else render->setDrawColor(guiStyle->colors.inactiveCaptionText);
	guiStyle->captionFont->draw(captionPosition, title);


	Vec2i contentPosition = getPosition() + contentAdjust;
	Vec2i contentSize = getSize() - contentAdjust;
	
	//Draw the objects
	render->setClipZone(contentPosition, contentSize);
	render->translate(contentPosition);
	Container::onDraw();
	render->translate(-contentPosition);
	render->disableClipZone();
}

//=============================================================================
// Called when user selects the window
//=============================================================================
void Window::onFocus()
{
	focused = true;
	//Container::onFocus();
}

//=============================================================================
// Called when user selects another window
//=============================================================================
void Window::onLoseFocus()
{
	focused = false;
	//Container::onLoseFocus();
}

//=============================================================================
// Called when the mouse leaves the window
//=============================================================================
void Window::onMouseOut()
{
	system->setCursor(System::CURSOR_ARROW);
	Container::onMouseOut();
}

//=============================================================================
// Called when the mouse is pressed on the element
//=============================================================================
void Window::onMousePressed()
{
	Vec2i captionMin(guiStyle->dimensions.windowBorder, guiStyle->dimensions.windowBorder);
	Vec2i captionMax = captionMin + Vec2i(getSize().getX() - guiStyle->dimensions.windowBorder * 2, guiStyle->dimensions.captionHeight);
	if(mousePosition.getX() >= captionMin.getX() && mousePosition.getY() >= captionMin.getY()) {
		if(mousePosition.getX() < captionMax.getX() && mousePosition.getY() < captionMax.getY()) {
			moving = true;
		}
	}

	Container::onMousePressed();
}

//=============================================================================
// Called when the mouse is released on the element
//=============================================================================
void Window::onMouseReleased()
{
	moving = false;
	Container::onMouseReleased();
}

//=============================================================================
// Called when the mouse moves
//=============================================================================
void Window::onMouseMove(const Vec2i &position)
{
	if(moving) {
		Vec2i delta = position - mousePosition;
		if(!delta.isZero()) setPosition(getPosition() + delta);
	} else {
		mousePosition = position;

		Vec2i contentPosition = position - Vec2i(guiStyle->dimensions.windowBorder, guiStyle->dimensions.windowBorder + guiStyle->dimensions.captionHeight);
		
		if(contentPosition.getX() < 0) system->setCursor(System::CURSOR_SIZEWE);
		else if(contentPosition.getX() > 0) system->setCursor(System::CURSOR_SIZEWE);
		else system->setCursor(System::CURSOR_ARROW);
		
		Container::onMouseMove(contentPosition);
	}
}
