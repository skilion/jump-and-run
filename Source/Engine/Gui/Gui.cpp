#include "Gui.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Gui::Gui()
{
	captionFont = nullptr;
	defaultFont = nullptr;
	bigFont = nullptr;
}

//=============================================================================
// Destructor
//=============================================================================
//Gui::~Gui() {}

//=============================================================================
// Initialize
//=============================================================================
bool Gui::initialize()
{
	colors.activeCaption.set(0, 0.63f, 0.91f, 1);
	colors.activeCaptionText.set(0, 0, 0, 1);
	colors.activeButton.set(0.92f, 0.96f, 1, 1);
	colors.activeButtonBorder = colors.activeCaption;
	colors.activeButtonText = colors.activeButtonBorder;
	colors.activeWindowBorder.set(0.24f, 0.28f, 0.8f, 1);
	colors.activeWindow.set(0.95f, 0.95f, 1, 1);
	colors.focusBorder.set(1, 0.95f, 0, 1);
	colors.labelText.set(0, 0, 0, 1);
	colors.inactiveCaption.set(0.44f, 0.57f, 0.74f, 1);
	colors.inactiveCaptionText.set(0, 0, 0, 1);
	colors.inactiveButton = colors.activeWindow;
	colors.inactiveButtonBorder.set(0.5f, 0.5f, 0.5f, 1);
	colors.inactiveButtonText.set(0, 0, 0, 1);
	colors.inactiveWindowBorder = colors.activeWindowBorder;
	colors.inactiveWindow.set(0.92f, 0.96f, 1, 1);

	dimensions.captionHeight	= 20;
	dimensions.windowBorder		= 3;

	dimensions.buttonSize.set(100, 24);
	dimensions.labelSize.set(200, 24);
	dimensions.windowSize.set(320, 240);

	captionFont	= Font::create("DejaVuSans-Bold.ttf", 10, false);
	defaultFont	= Font::create("DejaVuSans.ttf", 10, false);
	bigFont		= Font::create("DejaVuSans.ttf", 16, false);

	return captionFont && defaultFont && bigFont;
}

//=============================================================================
// Shutdown
//=============================================================================
void Gui::shutdown()
{
	delete bigFont;
	delete defaultFont;
	delete captionFont;
}
