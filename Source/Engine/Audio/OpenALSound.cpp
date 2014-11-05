#include <AL/al.h>
#include <vorbis/vorbisfile.h>

#include "Log.h"
#include "Memory.h"
#include "Platform.h"
#include "OpenALSound.h"

#include "memdbg.h"

using namespace myengine;


unsigned char *OpenALSound::data = nullptr;


//=============================================================================
// Constructor
//=============================================================================
OpenALSound::OpenALSound()
{
	audioStream = nullptr;
	format = 0;
	source = 0;
	playing = looping = false;

	if(!data) data = new unsigned char[BUFFER_SIZE];

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	//alSourcefv(source, AL_POSITION, sourcePos);
	//alSourcefv(source, AL_VELOCITY, sourceVel);
	alSourcei(source, AL_LOOPING, false);
	alGenBuffers(BUFFERS_NO, buffers);
}

//=============================================================================
// Destructor
//=============================================================================
OpenALSound::~OpenALSound()
{
	free();

	alDeleteBuffers(BUFFERS_NO, buffers);
	alDeleteSources(1, &source);

	//delete [] data;
}

//=============================================================================
// Returns if the sound is paused
//=============================================================================
/*bool OpenALSound::isPaused() const
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return AL_PAUSED == state;
}*/

//=============================================================================
// Returns if the sound is playing
//=============================================================================
/*bool OpenALSound::isPlaying() const
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return AL_PLAYING == state;
}*/

//=============================================================================
// Returns if the sound is stopped
//=============================================================================
/*bool OpenALSound::isStopped() const
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return (AL_STOPPED == state) || (AL_INITIAL == state);
}*/

//=============================================================================
// Returns if the sound is looping
//=============================================================================
/*bool OpenALSound::isLooping() const
{
	return looping;
}*/

//=============================================================================
// Plays the sound
//=============================================================================
void OpenALSound::play(bool loop)
{
	stop();
	audioStream->rewind();

	unsigned count;
	for(count = 0; count < BUFFERS_NO; count++) {
		if(!stream(buffers[count])) break;
	}

	alSourceQueueBuffers(source, count, buffers);
	alSourcePlay(source);

	playing = true;
	looping = loop;
}

//=============================================================================
// Pauses the sound
//=============================================================================
/*void OpenALSound::pause()
{
	alSourcePause(source);
}*/

//=============================================================================
// Rewinds the sound
//=============================================================================
/*void OpenALSound::rewind()
{
	stop();
	play();
}*/

//=============================================================================
// Stops the sound
//=============================================================================
void OpenALSound::stop()
{
	playing = false;

	alSourceStop(source);

	ALint queued;
	alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
	while(queued--) {
		ALuint buffer;
		alSourceUnqueueBuffers(source, 1, &buffer);
	}
}

//=============================================================================
// Sets the sound gain
//=============================================================================
void OpenALSound::setGain(float gain)
{
	alSourcef(source, AL_GAIN, gain);
}

//=============================================================================
// Loads a sound file
//=============================================================================
bool OpenALSound::load(AudioStream *audioStream)
{
	free();

	if(audioStream && audioStream->isValid()) {
		this->audioStream = audioStream;
		frequency = audioStream->getFrequency();

		switch(audioStream->getBitsPerSample())
		{
		case 8:
			switch(audioStream->getChannelsNo())
			{
			case 1:
				format = AL_FORMAT_MONO8;
				break;
			case 2:
				format = AL_FORMAT_STEREO8;
				break;
			default:
				return false;
			}
			break;

		case 16:
			switch(audioStream->getChannelsNo())
			{
			case 1:
				format = AL_FORMAT_MONO16;
				break;
			case 2:
				format = AL_FORMAT_STEREO16;
				break;
			default:
				return false;
			}
			break;

		default:
			return false;
		}
	}

	return true;
}

//=============================================================================
// Deletes the audiostream
//=============================================================================
void OpenALSound::free()
{
	format = 0;

	if(audioStream) {
		delete audioStream;
		audioStream = nullptr;
	}
}

//=============================================================================
// Update the source
//=============================================================================
void OpenALSound::update()
{
	if(audioStream && audioStream->isValid()) {
		int processed;
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

		while(processed--) {
			ALuint buffer;
			alSourceUnqueueBuffers(source, 1, &buffer);

			if(!stream(buffer)) {
				if(looping) {
					audioStream->rewind();
					stream(buffer);
				} else {
					playing = false;
					continue; //Unqueue all buffers anyway
				}
			}

			alSourceQueueBuffers(source, 1, &buffer);
		}

		//Ensure the source keep playing
		if(playing) {
			ALint state;
			alGetSourcei(source, AL_SOURCE_STATE, &state);
			if(AL_STOPPED == state) alSourcePlay(source);
		}
	}
}

//=============================================================================
// Fills the specified buffer with the data from the audio stream
//=============================================================================
bool OpenALSound::stream(ALuint buffer)
{
	unsigned read = audioStream->get(data, BUFFER_SIZE);
	if(read) alBufferData(buffer, format, data, read, frequency);
	//TODO: should check for out of memory ?
	return read != 0;
}
