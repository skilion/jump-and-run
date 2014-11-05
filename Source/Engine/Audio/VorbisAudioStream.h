#pragma once

#include <vorbis/vorbisfile.h>

#include "AudioStream.h"


namespace myengine {
	
	class VorbisAudioStream: public AudioStream
	{
	public:
			VorbisAudioStream();
			~VorbisAudioStream();

		//Stream interface
		virtual bool			isValid() const;
		virtual unsigned		get(unsigned char *data, unsigned size);
		virtual void			rewind();

		//AudioStream interface
		virtual unsigned		getBitsPerSample();
		virtual unsigned		getChannelsNo();
		virtual unsigned		getFrequency();

		bool					open(const char *filename);
		void					close();


	private:
		void			logError(int code);

		bool			valid;
		OggVorbis_File	vf;
		//HACK: vorbis file writes after vf ?
		int	padding[4];
	};

}
