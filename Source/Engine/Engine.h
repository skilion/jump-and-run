#pragma once
#ifndef __ENGINE_H__
#define __ENGINE_H__


namespace myengine {

	class Engine
	{
	public:
			//Engine();
			//~Engine();

		bool		initialize();
		void		shutdown();
		
		void		exit();

		bool		isPaused();
		void		setPause(bool pause);
		
		static int	main();


	private:
		int		run();

		bool	running;
		bool	pause;
	};

	extern Engine *engine;

		
	//==========================================================================
	// Shutdown the engine
	//=========================================================================
	inline void Engine::exit()
	{
		running = false;
	}

	//=========================================================================
	// Returns true if the engine is paused
	//=========================================================================
	inline bool Engine::isPaused()
	{
		return pause;
	}

	//=========================================================================
	// Sets if the engine is paused
	//=========================================================================
	inline void Engine::setPause(bool pause)
	{
		this->pause = pause;
	}

}


#endif //!_ENGINE_H_