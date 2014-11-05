#pragma once

namespace myengine {

	template<class T> class Sender;
	template<unsigned SIZE, typename T> class Vector;
	typedef Vector<2, int> Vec2i;
	
	class MouseEvent
	{
	public:
			MouseEvent(int x, int y);
			//~MouseEvent();

		void				get(int &x, int &y) const;
		void				get(Vec2i &v) const;
		int					getX() const;
		int					getY() const;


	private:
		int		x, y;
	};

	extern Sender<MouseEvent> mouseEventSender;

}
