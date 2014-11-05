#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__


namespace myengine {

	class Audio
	{
	public:
			//Audio();
			virtual ~Audio();

		static Audio		*create();

		virtual bool		initialize() = 0;
		virtual void		shutdown() = 0;

		virtual void		resume() = 0;
		virtual void		suspend() = 0;

		virtual void		update() = 0;
		
		virtual unsigned	loadSound(const char *filename) = 0;
		virtual void		unloadSound(unsigned sound) = 0;
		
		virtual void		playSound(unsigned sound) = 0;
		virtual void		loopSound(unsigned sound) = 0;
		virtual void		stopSound(unsigned sound) = 0;
		virtual void		setSoundGain(unsigned sound, float gain) = 0; //0.0 = mute, 0.5 = -6db, 1.0 = normal (scalar amplitude multiplier)
	};

	extern Audio *audio;

}


#endif //!__AUDIO_H__
