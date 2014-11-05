#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "Audio.h"
#include "OpenALSound.h"

#include "Containers/Array.h"


namespace myengine {

	class OpenALAudio: public Audio
	{
	public:
			OpenALAudio();
			//virtual ~OpenALAudio();

		virtual bool		initialize();
		virtual void		shutdown();

		virtual void		resume();
		virtual void		suspend();

		virtual void		update();

		virtual unsigned	loadSound(const char *filename);
		virtual void		unloadSound(unsigned sound);

		virtual void		playSound(unsigned sound);
		virtual void		loopSound(unsigned sound);
		virtual void		stopSound(unsigned sound);
		virtual void		setSoundGain(unsigned sound, float gain);


	private:
		bool		checkErrorAL();
		bool		checkErrorALC();

		bool			started;
		ALCdevice		*device;
		ALCcontext		*context;

		Array<OpenALSound*>		sounds;
		unsigned				soundsNo;
	};

}
