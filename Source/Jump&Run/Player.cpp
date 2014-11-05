#include "Const.h"
#include "World.h"
#include "Player.h"
#include "GameLocal.h"
#include "ParticleSystem.h"

using namespace myengine;


const float JUMP_HEIGHT_PIXEL = 150;
const float JUMP_SPEED = 0.08f;


//=============================================================================
// Constructor
//=============================================================================
Player::Player()
{
	setSize(Vec2i(20, 48));
	setTexture(TEX_PLAYER_START, TEX_PLAYER_END - TEX_PLAYER_START);
}

//=============================================================================
// Destructor
//=============================================================================
//Player::~Player() {}

//=============================================================================
// Spawns the player
//=============================================================================
void Player::spawn()
{
	onGround = false;
	jumping = 0;

	Entity::spawn();
}

//=============================================================================
// Kills the player
//=============================================================================
void Player::kill()
{
	//Reset jump
	onGround = false;
	jumping = 0;

	if(!isDead()) {
		#ifndef NO_BLOOD
		particleSystem.createEffect(ParticleSystem::EFFECT_BLOOD_DROP, getPosition());
		#endif //!NO_BLOOD

		audio->playSound(sounds[SOUND_END_GAME]);
	}

	Entity::kill();
}

//=============================================================================
// Sets the world where the entity can move
//=============================================================================
void Player::jump()
{
	if(onGround && !isDead()) {
		jumping = PI / 2;
		jumped = 0;
		onGround = false;
		audio->playSound(sounds[SOUND_JUMP]);
	}
}

//=============================================================================
// Updates the entity
//=============================================================================
void Player::tick()
{
	if(jumping > 0)	//Jumping
	{
		jumping -= JUMP_SPEED;
		int height = static_cast<int>(cos(jumping) * JUMP_HEIGHT_PIXEL);

		//Stop jumping if the player hits something
		if(!move(Vec2i(0, jumped - height))) jumping = 0;
		
		jumped = height;
	}
	else	//Falling
	{
		if(applyGravity()) onGround = false;
		else onGround = true;
	}
	
	//Kill if out of the world
	if(getPosition().getY() > SCREEN_Y_PIXEL + 10) kill();
}

//=============================================================================
// Manages player's collisions
//=============================================================================
bool Player::checkCollision()
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
			case TILE_LETHAL:
				kill();
				break;

			case TILE_COIN:
				tile.tile2 = 0;
				tile.type = TILE_NORMAL;
				world.setTile(pos, tile);
				audio->playSound(sounds[SOUND_COIN]);
				gameLocal->addScore(10);
				break;

			case TILE_DIAMOND:
				tile.tile2 = 0;
				tile.type = TILE_NORMAL;
				world.setTile(pos, tile);
				audio->playSound(sounds[SOUND_DIAMOND]);
				gameLocal->addScore(100);
				break;

			case TILE_END_LEVEL:
				gameLocal->nextLevel();
			case TILE_SOLID:
				return true;
			}
		}
	}

	return false;
}