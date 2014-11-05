#pragma once

#include <Containers/List.h>

#include "Object.h"


namespace myengine {
	namespace gui {

		class Container: public Object
		{
		public:
				Container(Container *parent);
				virtual ~Container();

			//Object interface
			virtual void	onChar(char c);
			virtual void	onDraw();
			virtual void	onMousePressed();
			virtual void	onMouseReleased();
			virtual void	onMouseMove(const Vec2i &position);

			void	add(Object *object);
			void	remove(Object *object);
			void	removeObjects();
			
			void	setFocused(Object *object);
			
			virtual void	onEvent(MsgEvent &msgEvent);
			//virtual void	onTick();
			

		private:
			void	setUndermouse(Object *object);

			Object	*focused, *undermouse;
			List<Object*>	objects;
		};

	}
}
