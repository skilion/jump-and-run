#pragma once

#include "Label.h"


namespace myengine {
	namespace gui {

		class Button: public Label
		{
		public:
				Button(Container *parent, const String &text);
				//virtual ~Button();

			//GuiWidget interface
			virtual void	onDraw();
			virtual void	onFocus();
			virtual void	onLoseFocus();
			virtual void	onMouseOut();
			virtual void	onMousePressed();
			virtual void	onMouseReleased();


		private:
			void	drawSelectionBorders();

			bool	focused;
			bool	pressed;
		};


		//=====================================================================
		// Gets the button command
		//=====================================================================
		/*inline int Button::getCommand()
		{
			return command;
		}

		//=====================================================================
		// Sets the button command
		//=====================================================================
		inline void Button::setCommand(int command)
		{
			this->command = command;
		}*/
	}
}
