#include "Render/Color.h"
#include "Render/Render.h"

#include "Gui.h"
#include "Label.h"

#include "memdbg.h"

using namespace myengine::gui;
using myengine::String;


//=============================================================================
// Constructor
//=============================================================================
Label::Label(Container *parent, const String &text): Object(parent)
{
	this->text = text;
	textAlignment = ALIGN_LEFT;
	borderType = BORDER_NONE;
	
	setSize(guiStyle->dimensions.labelSize);
}

//=============================================================================
// Destructor
//=============================================================================
//Label::~Label() {}

//=============================================================================
// Called when the label changes size
//=============================================================================
void Label::onChangeSize()
{
	updateTextPosition();
}

//=============================================================================
// Draws the label
//=============================================================================
void Label::onDraw()
{
	//Caption
	render->setClipZone(getPosition(), getSize());
	render->setDrawColor(guiStyle->colors.labelText);
	guiStyle->defaultFont->draw(getPosition() + textPosition, text);
	render->disableClipZone();

	switch(borderType)
	{
	case BORDER_SOLID:
		render->setDrawColor(Color(0, 0, 0, 1));
		render->drawOutlinedRect(getPosition(), getSize());
		break;
	}
	/*int x1, y1, x2, y2;
	getRelativeBounds(x1, y1, x2, y2);

	GuiStyle *guiStyle = gui->getStyle();

	//Borders
	if(bordertype != BORDER_NONE)
	{
		render->setDrawColor(guiStyle->getColor(GuiStyle::C_BORDER));*/

		/*switch(bordertype) {
		case BORDER_SIMPLE:
			render->setDrawColor(guiStyle->getColor(Gui::COLOR_BRIGHT));
			break;
		case BORDER_EMBOSSED:
			render->setDrawColor(guiStyle->getColor(Gui::COLOR_BRIGHT));
			break;
		case BORDER_IMPRESSED:
			render->setDrawColor(guiStyle->getColor(Gui::COLOR_DARK));
			break;
		}*/

		//render->drawLine(x1, y1, x2 - 1, y1);	//top
		//render->drawLine(x1, y1, x1, y2 - 1);	//left

		/*switch(bordertype) {
		case BORDER_SIMPLE:
			render->setDrawColor(guiStyle->getColor(Gui::COLOR_BRIGHT));
			break;
		case BORDER_EMBOSSED:
			render->setDrawColor(guiStyle->getColor(Gui::COLOR_DARK));
			break;
		case BORDER_IMPRESSED:
			render->setDrawColor(guiStyle->getColor(Gui::COLOR_BRIGHT));
			break;
		}*/

		//render->drawLine(x1, y2, x2, y2);	//bottom
		//render->drawLine(x2, y1, x2, y2);	//right
	//}
}

//=============================================================================
// Gets the label text
//=============================================================================
const String &Label::getText() const
{
	return text;
}

//=============================================================================
// Sets the label text
//=============================================================================
void Label::setText(const String &text)
{
	this->text = text;
	updateTextPosition();
}

//=============================================================================
// Sets the text alignment
//=============================================================================
void Label::setTextAlignment(TextAlignment textAlignment)
{
	this->textAlignment = textAlignment;
	updateTextPosition();
}

//=============================================================================
// Sets the border type
//=============================================================================
void Label::setBorderType(BorderType borderType)
{
	this->borderType = borderType;
}

//=============================================================================
// Update the text position
//=============================================================================
void Label::updateTextPosition()
{
	const Font *font = guiStyle->defaultFont;
	int captionWidth = font->getStringWidth(text);
	int captionHeight = font->getHeight();

	//Horizontal alignment
	switch(textAlignment)
	{
	case ALIGN_LEFT:
		textPosition.setX(2);
		break;

	case ALIGN_CENTER:
		textPosition.setX((getSize().getX() - captionWidth) / 2);
		break;

	case ALIGN_RIGHT:
		textPosition.setX(getSize().getX() - captionWidth - 2);
		break;
	}
	
	//Vertical alignment
	textPosition.setY((getSize().getY() - captionHeight) / 2 - 1);

	/*if((alignment & 12) & ALIGN_VCENTER)
	{*/
		//textY = (height - captionHeight) / 2 - 1;
	/*}
	else
	{
		if((alignment & 12) & ALIGN_UP)
		{
			textY = 2;
		}
		else	//ALIGN_DOWN
		{
			textY = height - captionHeight - 2;
		}
	}*/
}
