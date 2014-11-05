#pragma once

namespace myengine {

	template<class T> class Sender;
	
	class CharEvent
	{
	public:
			CharEvent(char c);
			//~CharEvent();

		char	getChar() const;

		
	private:
		char	c;
	};

	extern Sender<CharEvent> charEventSender;

}
