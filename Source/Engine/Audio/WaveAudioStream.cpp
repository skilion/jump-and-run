#include "Log.h"
#include "Memory.h"
#include "WaveAudioStream.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
WaveAudioStream::WaveAudioStream()
{
	valid = false;
	file = nullptr;
	Memory::memset(&waveHeader, 0, sizeof(WaveHeader));
}

//=============================================================================
// Destructor
//=============================================================================
WaveAudioStream::~WaveAudioStream()
{
	close();
}

//=============================================================================
// Returns true if the stream is valid
//=============================================================================
bool WaveAudioStream::isValid() const
{
	return valid;
}

//=============================================================================
// Reads the specified bytes from the stream, returns how many bytes has been effectively read
//=============================================================================
unsigned WaveAudioStream::get(unsigned char *data, unsigned size)
{
	if(!file) return 0;
	return file->read(data, size);
}

//=============================================================================
// Rewinds the stream
//=============================================================================
void WaveAudioStream::rewind()
{
	if(file) file->setPosition(sizeof(WaveHeader));
}

//=============================================================================
// Returns the numbers of bits per sample
//=============================================================================
unsigned WaveAudioStream::getBitsPerSample()
{
	return waveHeader.bitsPerSample;
}

//=============================================================================
// Returns the numbers of channels
//=============================================================================
unsigned WaveAudioStream::getChannelsNo()
{
	return waveHeader.numChannels;
}

//=============================================================================
// Returns the frequency
//=============================================================================
unsigned WaveAudioStream::getFrequency()
{
	return waveHeader.sampleRate;
}

//=============================================================================
// Opens a ogg vorbis file
//=============================================================================
bool WaveAudioStream::open(const char *filename)
{
	close();

	file = File::createFile();
	file->open(filename, File::OPENMODE_INPUT);
	if(!file->isOpen()) return false;
	
	//Read the wave header
	file->read(reinterpret_cast<unsigned char*>(&waveHeader), sizeof(WaveHeader));

	//Some checks
	bool unsupported = false;
	if(waveHeader.chunkID != 0x46464952)		unsupported = true;	//'RIFF'
	if(waveHeader.format != 0x45564157)			unsupported = true;	//'WAVE'
	if(waveHeader.subchunk1ID != 0x20746D66)	unsupported = true;	//'fmt '
	if(waveHeader.subchunk2ID != 0x61746164)	unsupported = true;	//'data'
	if(waveHeader.audioFormat != 1)				unsupported = true;	//only PCM
	if(unsupported) return false;

	valid = true;
	
	return true;
}

//=============================================================================
// Close the file
//=============================================================================
void WaveAudioStream::close()
{
	valid = false;

	if(file) {
		file->close();
		delete file;
		file = nullptr;
	}

	Memory::memset(&waveHeader, 0, sizeof(WaveHeader));
}
