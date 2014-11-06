#include "Log.h"
#include "Game.h"
#include "Audio.h"
#include "Engine.h"
#include "Random.h"
#include "System.h"

#include "Config/Config.h"
#include "Gui/Gui.h"
#include "Render/Render.h"

#include "memdbg.h"

using namespace myengine;


//Main classes
Engine		*myengine::engine	= nullptr;
System		*myengine::system	= nullptr;
Log		*myengine::log		= nullptr;
Audio		*myengine::audio	= nullptr;
Render		*myengine::render	= nullptr;
Gui		*myengine::guiStyle	= nullptr;
Game		*myengine::game		= nullptr;


//=============================================================================
// Constructor
//=============================================================================
//Engine::Engine() {}

//=============================================================================
// Destructor
//=============================================================================
//Engine::~Engine() {}

//=============================================================================
// Initializes
//=============================================================================
bool Engine::initialize()
{
	system		= System::create();
#ifdef DEBUG
	log			= Log::create();
#else
	log = new Log();
#endif
	render		= Render::create();
	audio		= Audio::create();
	//guiStyle	= new Gui;

	//Initialize random number generator
	random.initialize(system->getMilliseconds() * system->getFreeSpaceMegaBytes());

	log->print("*** Engine started ***");
	return true;
}


//=============================================================================
// Shutdowns
//=============================================================================
void Engine::shutdown()
{
	log->print("*** Engine shutdown ***");

	delete guiStyle;
	delete game;
	delete audio;
	delete render;
	delete log;
	delete system;
}

//=============================================================================
// Engine loop
//=============================================================================
const int FPS = 60;
int Engine::run()
{
	initialize();

	game = Game::create();
	
	//Read the configuration file
	log->print("Loading configuration...");
	config.readFromFile("config.txt");

	bool error = false;
	if(!render->initialize()) error = true;
	audio->initialize();
	//if(!guiStyle->initialize()) error = true;
	if(!game->initialize()) error = true;

	if(!error) {
		log->print("*** Game started ***");
		running = true;
		pause = false;

		unsigned time, lastFrameTime = 0, elapsedTime/*, frame = 0*/;
		while(running)
		{
			system->handleEvents();

			if(!pause) {
				audio->update();

				//Compute the elapsed time
				time = system->getMilliseconds();
				if(time < lastFrameTime) {
					elapsedTime = 0xFFFFFFFF - lastFrameTime + time;
				} else {
					elapsedTime = time - lastFrameTime;
				}

				if(elapsedTime >= (1000 / FPS)) {
					game->frame(elapsedTime);
					lastFrameTime = time;
					//frame++;
				}

				/*if(elapsedTime >= 5000 ) {
					log->printf("time: %i - frames: %i - FPS: %i", elapsedTime, frame, frame / 5);
					frame = 0;
					lastFrameTime = time;
				}*/
			}

			system->sleep(5);
		}

		log->print("*** Game shutdown ***");
	}

	game->shutdown();
	//guiStyle->shutdown();
	render->shutdown();
	audio->shutdown();

	if(error) system->pauseTillExit(); //ensure the user see the error //todo: win console do not send quit msg...

	log->print("Saving configuration...");
	config.saveToFile("config.txt");

	shutdown();

	if(error) return 1;
	return 0;
}

//=============================================================================
// Engine entry point
//=============================================================================
int Engine::main()
{
	/*#ifdef _DEBUG
	test();
	#endif*/

	engine = new Engine;
	int returnCode = engine->run();
	delete engine;

	return returnCode;
}
