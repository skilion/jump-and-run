#include <Random.h>

#include "Const.h"
#include "Enemy.h"
#include "World.h"
#include "GameLocal.h"
#include "ParticleSystem.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Enemy::Enemy()
{
	enemyType = ENEMY_NULL;
}

//=============================================================================
// Destructor
//=============================================================================
//Enemy::~Enemy() {}

//=============================================================================
// Kills the enemy
//=============================================================================
void Enemy::kill()
{
	#ifndef NO_BLOOD
	if(!isDead()) {
		int squish = SOUND_SQUISH_START + random.getUnsigned(SOUND_SQUISH_END - SOUND_SQUISH_START);
		audio->playSound(sounds[squish]);
		particleSystem.createEffect(ParticleSystem::EFFECT_BLOOD_SPLASH, getPosition());
	}
	#endif //!NO_BLOOD

	Entity::kill();
}

//=============================================================================
// Spawns an enemy according to the type
//=============================================================================
void Enemy::spawn(EnemyType enemyType)
{
	this->enemyType = enemyType;
	goingLeft = true;
	
	switch(enemyType)
	{
	case ENEMY_GREEN_BALL:
		killable = true;
		fly = false;
		speed = 2;
		setSize(Vec2i(32, 32));
		setTexture(TEX_GREEN_BALL_START, TEX_GREEN_BALL_END - TEX_GREEN_BALL_START);
		break;

	case ENEMY_EVIL_MAN:
		killable = true;
		fly = false;
		speed = 3;
		setSize(Vec2i(24, 59));
		setTexture(TEX_EVIL_MAN_START, TEX_EVIL_MAN_END - TEX_EVIL_MAN_START);
		break;

	case ENEMY_SUPER_BALL:
		killable = false;
		fly = false;
		speed = 1;
		setSize(Vec2i(32, 36));
		setTexture(TEX_SUPER_BALL_START, TEX_SUPER_BALL_END - TEX_SUPER_BALL_START);
		break;

	case ENEMY_FLY_BALL:
		killable = true;
		fly = true;
		speed = 2;
		setSize(Vec2i(28, 32));
		setTexture(TEX_FLY_BALL_START, TEX_FLY_BALL_END - TEX_FLY_BALL_START);
		break;
	}

	Entity::spawn();
}

//=============================================================================
// Respawn the enemy
//=============================================================================
void Enemy::respawn()
{
	goingLeft = true;
	Entity::spawn();
}
//=============================================================================
// Check if the enemy collides
//=============================================================================
bool Enemy::checkCollision()
{
	Vec2i start, end;
	world.pixelToMap(getPosition(), start);
	world.pixelToMap(getPosition() + getSize(), end);

	for(int x = start.getX(); x <= end.getX(); x++) {
		for(int y = start.getY(); y <= end.getY(); y++) {
			Vec2i pos(x, y);
			Tile tile = world.getTile(pos);
			switch(tile.type)
			{
			case TILE_SOLID:
			case TILE_ENEMY_STOP:
				return true;
			case TILE_LETHAL:
				kill();
				break;
			}
		}
	}

	return false;
}

//=============================================================================
// Returns true if the enemy is killable
//=============================================================================
bool Enemy::isKillable()
{
	return killable;
}

//=============================================================================
// Update the enemy
//=============================================================================
void Enemy::tick()
{
	if(isDead()) return;

	if(!fly && applyGravity()) return; //Falling

	//Movement
	bool stop = false;
	stop = !move(Vec2i(goingLeft ? -speed : speed, 0));
	if(stop) goingLeft = !goingLeft;
}

//=============================================================================
// Reads the enemy from the specified file
//=============================================================================
void Enemy::readFromFile(File *file)
{
	file->readInt(reinterpret_cast<int&>(enemyType));
	if(enemyType != ENEMY_NULL) spawn(enemyType);
	Entity::readFromFile(file);
}

//=============================================================================
// Save the enemy in the specified file
//=============================================================================
void Enemy::writeToFile(File *file) const
{
	file->writeInt(enemyType);
	Entity::writeToFile(file);
}

//=============================================================================
// Returns the name of the enemy
//=============================================================================
const char *Enemy::getEnemyTypeName(EnemyType type)
{
	switch(type)
	{
	case ENEMY_GREEN_BALL:	return "green ball";
	case ENEMY_EVIL_MAN:	return "evil man";
	case ENEMY_SUPER_BALL:	return "super ball";
	case ENEMY_FLY_BALL:	return "fly ball";
	}

	return 0;
}