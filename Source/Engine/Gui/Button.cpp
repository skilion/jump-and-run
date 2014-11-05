#include "Render/Color.h"
#include "Render/Render.h"

#include "Gui.h"
#include "Button.h"
#include "MsgEvent.h"
#include "Container.h"

using namespace myengine::gui;
using myengine::String;
using myengine::Vec2i;


//=============================================================================
// Constructor
//=============================================================================
Button::Button(Container *parent, const String &text): Label(parent, text)
{
	focused = false;
	pressed = false;

	setSize(guiStyle->dimensions.buttonSize);
	setTextAlignment(ALIGN_CENTER);
}

//=============================================================================
// Destructor
//=============================================================================
//Button::~Button() {}

//=============================================================================
// Draws the button
//=============================================================================
void Button::onDraw()
{
	//Background
	if(pressed) render->setDrawColor(guiStyle->colors.activeButton);
	else render->setDrawColor(guiStyle->colors.inactiveButton);
	render->drawFilledRect(getPosition(), getSize());

	//Draw text
	render->setClipZone(getPosition(), getSize());
	if(pressed) render->setDrawColor(guiStyle->colors.activeButtonText);
	else render->setDrawColor(guiStyle->colors.inactiveButtonText);
	guiStyle->defaultFont->draw(getPosition() + textPosition, text);
	render->disableClipZone();

	//Border
	if(pressed) render->setDrawColor(guiStyle->colors.activeButtonBorder);
	else render->setDrawColor(guiStyle->colors.inactiveButtonBorder);
	render->drawOutlinedRect(getPosition(), getSize());

	if(focused) drawSelectionBorders();
}

//=============================================================================
// Sets the button state unpressed
//=============================================================================
void Button::onMouseOut()
{
	pressed = false;
}

//=============================================================================
// Sets the button state pressed
//=============================================================================
void Button::onMousePressed()
{
	pressed = true;
}

//=============================================================================
// Sends the command to the parent window and sets the button unpressed
//=============================================================================
void Button::onMouseReleased()
{
	if(pressed)	{
		pressed = false;
		MsgEvent msgEvent(this);
		parent->onEvent(msgEvent);
	}
}

//=============================================================================
// Called when user selects the control
//=============================================================================
void Button::onFocus()
{
	focused = true;
}

//=============================================================================
// Called when user selects another control
//=============================================================================
void Button::onLoseFocus()
{
	focused = false;
}

//=============================================================================
// Draws the selection borders
//=============================================================================
void Button::drawSelectionBorders()
{
	render->setDrawColor(guiStyle->colors.focusBorder);
	render->drawOutlinedRect(getPosition() - Vec2i(1, 1), getSize() + Vec2i(2, 2));
}
