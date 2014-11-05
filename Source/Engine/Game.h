#pragma once
#ifndef __GAME_H__
#define __GAME_H__


namespace myengine {
	
	class Game
	{
	public:
			//Game();
			virtual ~Game();
			
		static Game		*create();
			
		virtual bool	initialize() = 0;
		virtual void	shutdown() = 0;
		
		virtual void	frame(unsigned elapsedTime) = 0;
	};

	extern Game		*game;

}


#endif //!__GAME_H__