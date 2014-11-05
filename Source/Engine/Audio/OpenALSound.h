#pragma once

#include "AudioStream.h"


namespace myengine {

	class OpenALSound
	{
	public:
			OpenALSound();
			~OpenALSound();

		bool	load(AudioStream *audioStream);
		
		void	play(bool loop);
		void	stop();
		void	setGain(float gain);
		
		void	update();


	private:
		void	free();
		bool	stream(ALuint buffer);

		const static unsigned BUFFERS_NO	= 4;	//16;
		const static unsigned BUFFER_SIZE	= 4096;	//65536;

		AudioStream		*audioStream;
		ALenum			format;
		ALsizei			frequency;
		ALuint			source, buffers[BUFFERS_NO];

		bool			playing, looping;

		static unsigned char *data;
	};

}
