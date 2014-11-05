#pragma once

#include "EventSystem/Receiver.h"

#include "Container.h"


namespace myengine {
	
	class CharEvent;
	class KeyEvent;
	class MouseEvent;

	namespace gui {

		class MainContainer: public Container, public Receiver<CharEvent>, public Receiver<KeyEvent>, public Receiver<MouseEvent>
		{
		public:
				MainContainer();
				//virtual ~MainContainer();

			//Receiver interface
			virtual void	onEvent(CharEvent &charEvent);
			virtual void	onEvent(KeyEvent &keyEvent);
			virtual void	onEvent(MouseEvent &mouseEvent);
		};

	}
}
