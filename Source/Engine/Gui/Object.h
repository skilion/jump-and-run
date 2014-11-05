#pragma once

#include <Math/Rect.h>
#include <Math/Vector.h>


namespace myengine {
	namespace gui {
	
		class Container;
		class MsgEvent;
	
		class Object
		{
		public:
				Object(Container *parent);
				virtual ~Object();
				
			bool			contains(const Vec2i &point) const;
		
			const Vec2i		&getPosition() const;
			const Vec2i		&getSize() const;

			void			setPosition(const Vec2i &position);
			void			setSize(const Vec2i &size);
							
			virtual void	onChar(char c);
			virtual void	onChangeSize();
			virtual void	onCreate();
			virtual void	onDestroy();
			virtual void	onDraw();
			virtual void	onFocus();
			virtual void	onLoseFocus();
			virtual void	onMouseOut();
			virtual void	onMouseOver();
			virtual void	onMousePressed();
			virtual void	onMouseReleased();
			virtual void	onMouseMove(const Vec2i &position);


		protected:
			Container	*parent;
			Vec2i		position, size;
		};


		//=====================================================================
		// Returns the position of the object
		//=====================================================================
		inline const Vec2i &Object::getPosition() const
		{
			return position;
		}

		//=====================================================================
		// Returns the size of the object
		//=====================================================================
		inline const Vec2i &Object::getSize() const
		{
			return size;
		}
	
		//=====================================================================
		// Sets the position of the object
		//=====================================================================
		inline void Object::setPosition(const Vec2i &position)
		{
			this->position = position;
		}

	}
}