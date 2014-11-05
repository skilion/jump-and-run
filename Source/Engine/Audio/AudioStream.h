#pragma once

#include "Stream.h"


namespace myengine {
	
	class AudioStream: public Stream
	{
	public:
			//AudioStream();
			//virtual ~AudioStream();

		virtual unsigned		getBitsPerSample() = 0;
		virtual unsigned		getChannelsNo() = 0;
		virtual unsigned		getFrequency() = 0;
	};

}
