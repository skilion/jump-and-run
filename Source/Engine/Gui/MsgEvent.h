#pragma once

namespace myengine {
	namespace gui {

		class Object;
		
		class MsgEvent
		{
		public:
				MsgEvent(Object *source);
				//~MsgEvent();

			Object	*getSource() const;


		private:
			Object	*source;
		};

	}
}
