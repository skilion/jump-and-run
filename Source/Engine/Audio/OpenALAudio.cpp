#include "Log.h"
#include "Memory.h"
#include "OpenALAudio.h"
#include "WaveAudioStream.h"
#include "VorbisAudioStream.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Creates the Audio class
//=============================================================================
Audio *Audio::create()
{
	return new OpenALAudio;
}


//=============================================================================
// Constructor
//=============================================================================
OpenALAudio::OpenALAudio()
{
	started = false;
	device = 0;
	context = 0;

	sounds.ensureSize(16);
}

//=============================================================================
// Destructor
//=============================================================================
//OpenALAudio::~OpenALAudio() {}

//=============================================================================
// Initialize
//=============================================================================
bool OpenALAudio::initialize()
{
	if(started) return true;
	log->print("Initializing OpenAL...");
	
	device = alcOpenDevice(0);
	if(!device) {
		log->print("Can't open a sound device");
		return false;
	}
	
	context = alcCreateContext(device, 0);
	if(!context) {
		log->print("Can't create an OpenAL context");
		return false;
	}

	if(!alcMakeContextCurrent(context)) {
		log->print("Can't set active the OpenAL context");
		return false;
	}

	if(checkErrorALC()) return false;

	log->printf(" vendor: %s", alGetString(AL_VENDOR));
	log->printf(" renderer: %s", alGetString(AL_RENDERER));
	log->printf(" version: %s", alGetString(AL_VERSION));
	//log->printf(" extensions: %s", alGetString(AL_EXTENSIONS));

	started = true;
	soundsNo = 0;

	return true;
}

//=============================================================================
// Shutdown
//=============================================================================
void OpenALAudio::shutdown()
{
	if(!started) return;
	log->print("Shutting down OpenAL...");
	
	alcMakeContextCurrent(0);
	alcDestroyContext(context);
	alcCloseDevice(device);

	context = 0;
	device = 0;
	started = false;
}

//=============================================================================
// Resumes audio processing
//=============================================================================
void OpenALAudio::resume()
{
	if(started) alcProcessContext(context);
}

//=============================================================================
// Suspends audio processing
//=============================================================================
void OpenALAudio::suspend()
{
	if(started) alcSuspendContext(context);
}

//=============================================================================
// Updates all sounds
//=============================================================================
void OpenALAudio::update()
{
	if(started) {
		for(unsigned i = 0; i < soundsNo; i++) {
			OpenALSound *sound = sounds[i];
			if(sound) sound->update();
		}
		checkErrorAL();
	}
}

//=============================================================================
// Loads a sound file
//=============================================================================
unsigned OpenALAudio::loadSound(const char *filename)
{
	if(!started) return 0;

	File *file = File::createFile();
	file->open(filename, File::OPENMODE_INPUT);

	if(!file->isOpen()) {
		log->printf("Sound file not found: %s", filename);
		delete file;
		return 0;
	}

	unsigned char header[4];
	file->read(header, sizeof(header));
	file->close();
	delete file;

	const unsigned char waveMagic[] = { 'R', 'I', 'F', 'F' };
	const unsigned char oggMagic[] = { 'O', 'g', 'g', 'S' };

	AudioStream *audioStream;
	if(Memory::memcmp(header, waveMagic, sizeof(waveMagic)) == 0) {
		WaveAudioStream *waveStream = new WaveAudioStream;
		if(!waveStream->open(filename)) {
			log->printf("Can't load this wave file: %s", filename);
			delete waveStream;
			return 0;
		}
		audioStream = waveStream;
	} else if(Memory::memcmp(header, oggMagic, sizeof(oggMagic)) == 0) {
		VorbisAudioStream *vorbisStream = new VorbisAudioStream;
		if(!vorbisStream->open(filename)) {
			log->printf("Can't load this ogg vorbis file: %s", filename);
			delete vorbisStream;
			return 0;
		}
		audioStream = vorbisStream;
	} else {
		log->printf("Sound file not supported: %s", filename);
		return 0;
	}

	OpenALSound *sound = new OpenALSound;
	if(!sound->load(audioStream)) {
		log->printf("Unsupported sound: %s", filename);
		delete audioStream;
		return 0;
	}

	if(checkErrorAL()) {
		delete sound;
		delete audioStream;
		return 0;
	}

	sounds.ensureSize(soundsNo + 1);
	sounds[soundsNo++] = sound;
	
	return soundsNo;
}

//=============================================================================
// Unloads a sound
//=============================================================================
void OpenALAudio::unloadSound(unsigned sound)
{
	if(started && sound) {
		delete sounds[sound - 1];
		sounds[sound - 1] = nullptr;
	}
}

//=============================================================================
// Plays a sound
//=============================================================================
void OpenALAudio::playSound(unsigned sound)
{
	if(started && sound) sounds[sound - 1]->play(false);
}

//=============================================================================
// Plays a sound in loop
//=============================================================================
void OpenALAudio::loopSound(unsigned sound)
{
	if(started && sound) sounds[sound - 1]->play(true);
}

//=============================================================================
// Stops a sound
//=============================================================================
void OpenALAudio::stopSound(unsigned sound)
{
	if(started && sound) sounds[sound - 1]->stop();
}

//=============================================================================
// Sets the sound gain
//=============================================================================
void OpenALAudio::setSoundGain(unsigned sound, float gain)
{
	if(started && sound) sounds[sound - 1]->setGain(gain);
}

//=============================================================================
// Checks for OpenAL errors
//=============================================================================
bool OpenALAudio::checkErrorAL()
{
	unsigned errorCode = alGetError();
	if(errorCode != AL_NO_ERROR) {
		const char *errorString;
		switch(errorCode)
		{
		case AL_INVALID_NAME:
			errorString ="invalid name";
			break;
		case AL_INVALID_ENUM:
			errorString = "invalid enum";
			break;
		case AL_INVALID_VALUE:
			errorString = "invalid value";
			break;
		case AL_INVALID_OPERATION:
			errorString = "invalid operation";
			break;
		case AL_OUT_OF_MEMORY:
			errorString = "out of memory";
			break;
		default:
			errorString = "unknow";
		}
		log->printf("OpenAL error: %s", errorString);
		return true;
	}
	return false;
}

//=============================================================================
// Checks for ALC errors
//=============================================================================
bool OpenALAudio::checkErrorALC()
{
	unsigned errorCode = alcGetError(device);
	if(errorCode != ALC_NO_ERROR) {
		const char *errorString;
		switch(errorCode)
		{
		case ALC_INVALID_DEVICE:
			errorString = "invalid device";
			break;
		case ALC_INVALID_CONTEXT:
			errorString = "invalid context";
			break;
		case ALC_INVALID_ENUM:
			errorString = "invalid enum";
			break;
		case ALC_INVALID_VALUE:
			errorString = "invalid value";
			break;
		case ALC_OUT_OF_MEMORY:
			errorString = "out of memory";
			break;
		default:
			errorString = "unknow";
		}
		log->printf("OpenAL ALC error: %s", errorString);
		return true;
	}
	return false;
}