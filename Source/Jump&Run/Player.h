#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Entity.h"


class Player: public Entity
{
public:
		Player();
		//~Player();

	void	spawn();
	void	kill();
	void	jump();
	void	tick();
		
	virtual bool	checkCollision();
	

private:
	bool		onGround;
	float		jumping;
	int			jumped;
};


#endif //!__PLAYER_H__