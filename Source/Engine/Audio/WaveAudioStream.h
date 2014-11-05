#pragma once

#include "Platform.h"
#include "AudioStream.h"

#include "File/File.h"


namespace myengine {
	
	class WaveAudioStream: public AudioStream
	{
	public:
			WaveAudioStream();
			~WaveAudioStream();

		//Stream inteface
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
		struct WaveHeader {
			dword	chunkID;
			dword	chunkSize;
			dword	format;
			dword	subchunk1ID;
			dword	subchunk1Size;
			word	audioFormat;
			word	numChannels;
			dword	sampleRate;
			dword	byteRate;
			word	blockAlign;
			word	bitsPerSample;
			dword	subchunk2ID;
			dword	subchunk2Size;
		};

		bool		valid;
		File		*file;
		WaveHeader	waveHeader;
	};

}
