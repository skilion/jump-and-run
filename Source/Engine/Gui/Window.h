#pragma once

#include <myString.h>

#include "Container.h"


namespace myengine {
	namespace gui {

		class Window: public Container
		{
		public:
				Window(Container *parent, const String &title);
				//virtual ~Window();	

			//Container interface
			virtual void	onChangeSize();
			virtual void	onDraw();
			virtual void	onFocus();
			virtual void	onLoseFocus();
			virtual void	onMouseOut();
			virtual void	onMousePressed();
			virtual void	onMouseReleased();
			virtual void	onMouseMove(const Vec2i &position);


		private:
			String	title;
			Vec2i	mousePosition;

			bool	focused;
			bool	moving;
		};

	}
}
