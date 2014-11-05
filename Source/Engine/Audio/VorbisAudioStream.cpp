#include "Log.h"
#include "VorbisAudioStream.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
VorbisAudioStream::VorbisAudioStream()
{
	valid = false;
}

//=============================================================================
// Destructor
//=============================================================================
VorbisAudioStream::~VorbisAudioStream()
{
	close();
}

//=============================================================================
// Returns true if the stream is valid
//=============================================================================
bool VorbisAudioStream::isValid() const
{
	return valid;
}

//=============================================================================
// Reads the specified bytes from the stream, returns how many bytes has been effectively read
//=============================================================================
unsigned VorbisAudioStream::get(unsigned char *data, unsigned size)
{
	unsigned finalSize = 0;
	int read;
	do {
		read = ov_read(&vf, reinterpret_cast<char*>(data) + finalSize, size - finalSize, false, 2, 1, 0);
		if(read > 0) finalSize += read;
		else if(read < 0) {
			logError(read);
			valid = false;
			break;
		}
	} while((finalSize < size) && (read != 0));

	return finalSize;
}

//=============================================================================
// Rewinds the stream
//=============================================================================
void VorbisAudioStream::rewind()
{
	if(ov_time_seek_lap(&vf, 0) != 0) valid = false;
}

//=============================================================================
// Returns the numbers of bits per sample
//=============================================================================
unsigned VorbisAudioStream::getBitsPerSample()
{
	return 16;
}

//=============================================================================
// Returns the numbers of channels
//=============================================================================
unsigned VorbisAudioStream::getChannelsNo()
{
	return ov_info(&vf, -1)->channels;
}

//=============================================================================
// Returns the frequency
//=============================================================================
unsigned VorbisAudioStream::getFrequency()
{
	return ov_info(&vf, -1)->rate;
}

//=============================================================================
// Opens a ogg vorbis file
//=============================================================================
bool VorbisAudioStream::open(const char *filename)
{
	close();
	FILE *f = fopen(filename, "rb");
	if(0 == ov_open_callbacks(f, &vf, 0, 0, OV_CALLBACKS_DEFAULT)) valid = true;
	return valid;
}

//=============================================================================
// Close the file
//=============================================================================
void VorbisAudioStream::close()
{
	if(valid) {
		valid = false;
		ov_clear(&vf);
	}
}

//=============================================================================
// Prints an error to the log
//=============================================================================
void VorbisAudioStream::logError(int code)
{
	const char *errorStr;
	switch(code)
	{
	case OV_HOLE:
		errorStr = "OV_HOLE";
		break;

	case OV_EBADLINK:
		errorStr = "OV_EBADLINK";
		break;

	case OV_EINVAL:
		errorStr = "OV_EINVAL";
		break;

	default:
		errorStr = "unknow";
	}

	log->printf("libvorbisfile error: %s", errorStr);
}
