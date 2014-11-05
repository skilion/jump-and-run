#pragma once

#include <Math/Vector.h>

#include "Render/Color.h"
#include "Render/Font.h"


namespace myengine {

	class Gui
	{
	public:
			Gui();
			//~Gui();

		bool			initialize();
		void			shutdown();
		
		//GUI colors
		struct Colors {
			Color		activeCaption;
			Color		activeCaptionText;
			Color		activeButton;
			Color		activeButtonBorder;
			Color		activeButtonText;
			Color		activeWindowBorder;
			Color		activeWindow;
			Color		focusBorder;
			Color		labelText;
			Color		inactiveCaption;
			Color		inactiveCaptionText;
			Color		inactiveButton;
			Color		inactiveButtonBorder;
			Color		inactiveButtonText;
			Color		inactiveWindowBorder;
			Color		inactiveWindow;
		} colors;

		//Default object dimensions
		struct Dimensions {
			int		captionHeight;
			int		windowBorder;
			Vec2i	buttonSize;
			Vec2i	labelSize;
			Vec2i	windowSize;
		} dimensions;

		Font	*captionFont;
		Font	*defaultFont;
		Font	*bigFont;

	private:
	};

	extern Gui *guiStyle;

}
