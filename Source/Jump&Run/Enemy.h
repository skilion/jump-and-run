#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__


#include <File/File.h>

#include "Entity.h"

using namespace myengine;


class Enemy: public Entity
{
public:
	enum EnemyType {
		ENEMY_NULL,
		ENEMY_GREEN_BALL,
		ENEMY_EVIL_MAN,
		ENEMY_SUPER_BALL,
		ENEMY_FLY_BALL
	};

		Enemy();
		//~Enemy();

	void	kill();
	void	spawn(EnemyType enemyType);
	void	respawn();

	virtual bool	checkCollision();

	bool			isKillable();

	void			tick();

	void			readFromFile(File *file);
	void			writeToFile(File *file) const;

	static const char	*getEnemyTypeName(EnemyType type);


private:
	EnemyType	enemyType;
	int			speed;
	bool		killable, fly, goingLeft;
};


#endif //!__ENEMY_H__