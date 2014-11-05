#pragma once

namespace myengine {

	class Sound
	{
	public:
			//Sound();
			virtual ~Sound();
			
		virtual void	play();
		virtual void	loop();
		virtual void	stop();

		virtual void	setGain(float gain);
	};

}
