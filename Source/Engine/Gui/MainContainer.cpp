#include "EventSystem/CharEvent.h"
#include "EventSystem/KeyEvent.h"
#include "EventSystem/MouseEvent.h"

#include "MainContainer.h"

#include "memdbg.h"

using namespace myengine;
using namespace myengine::gui;


//=============================================================================
// Constructor
//=============================================================================
MainContainer::MainContainer(): Container(nullptr)
{
}

//=============================================================================
// Destructor
//=============================================================================
//MainContainer::~MainContainer() {}

//=============================================================================
// Manages CharEvents
//=============================================================================
void MainContainer::onEvent(CharEvent &charEvent)
{
	onChar(charEvent.getChar());
}

//=============================================================================
// Manages KeyEvents
//=============================================================================
void MainContainer::onEvent(KeyEvent &keyEvent)
{
	switch(keyEvent.getKeyCode())
	{
	case KeyEvent::MK_LEFT:
		switch(keyEvent.getKeyState())
		{
		case KeyEvent::KEY_RELEASE:
			onMouseReleased();
			break;

		//case KeyEvent::KEY_PRESS:
		default:
			onMousePressed();
		}
		break;
	}
}

//=============================================================================
// Manages MouseEvents
//=============================================================================
void MainContainer::onEvent(MouseEvent &mouseEvent)
{
	Vec2i position;
	mouseEvent.get(position);
	onMouseMove(position);
}