#include <Math/Math.h>

#include "Const.h"
#include "Entity.h"
#include "GameLocal.h"

using namespace myengine;


const Vec2i GRAVITIY(0, 6);


//=============================================================================
// Constructor
//=============================================================================
Entity::Entity()
{
	dead = true;

	start = len = 0;
	frame = wait = 0;
	flip = false;
}

//=============================================================================
// Destructor
//=============================================================================
//Entity::~Entity() {}

//=============================================================================
// Returns true if the entity is dead
//=============================================================================
bool Entity::isDead() const
{
	return dead;
}

//=============================================================================
// Returns the entity position
//=============================================================================
const Vec2i &Entity::getPosition() const
{
	return position;
}

//=============================================================================
// Returns the entity size
//=============================================================================
const Vec2i &Entity::getSize() const
{
	return size;
}

//=============================================================================
// Returns a rect that represents the entity
//=============================================================================
Rect Entity::getRect() const
{
	return Rect(position + Vec2i(2, 2), position + size - Vec2i(4, 4));
}

//=============================================================================
// Sets the entity position
//=============================================================================
void Entity::setPosition(const Vec2i &position)
{
	this->position = position;
}

//=============================================================================
// Sets the entity position
//=============================================================================
void Entity::setStartPosition(const Vec2i &position)
{
	startPosition = position;
}

//=============================================================================
// Move the entity pixel by pixel
//=============================================================================
bool Entity::move(const Vec2i &movement)
{
	bool completed = true;
	Vec2i realMovement, oldPosition;

	for(int i = 0; i < 2; i++) //Loop trought X and Y
	{ 
		while(realMovement[i] != movement[i]) {
			oldPosition = position;

			//Move by a pixel
			if(realMovement[i] > movement[i]) {
				realMovement[i]--;
				position[i]--;
			} else {
				realMovement[i]++;
				position[i]++;
			}

			//Check if the entity collides
			if(checkCollision()) {
				position = oldPosition;
				completed = false;
				break;
			}
		}
	}

	//Change the animation frame
	if(completed) {
		if(movement.getY() < 0) frame = 0; //Jumping
		else if(movement.getY() > 0) frame = 1;	//Falling
		else {	//Walking
			if(wait) wait--;
			else {
				frame++;
				if(frame > len) frame = 1;
				wait = 6;
			}
			if(movement.getX() < 0) flip = true;		//Walking left
			else if(movement.getX() > 0) flip = false;	//Walking right
		}
	}

	return completed;
}

//=============================================================================
// Draws the entity
//=============================================================================
void Entity::draw(int pixelShift)
{
	Vec2i shift(pixelShift, 0);
	if(flip) {
		textures[start + frame]->drawTile(position - shift, size, Vec2f(1, 0), Vec2f(0, 1));
	} else {
		textures[start + frame]->drawTile(position - shift, size);
	}
}

//=============================================================================
// Reads the entity from the specified file
//=============================================================================
void Entity::readFromFile(File *file)
{
	frame = wait = 0;
	flip = false;

	file->readChar(reinterpret_cast<char&>(dead));
	file->readUnsignedInt(start);
	file->readUnsignedInt(len);
	startPosition.readFromFile(file);
	size.readFromFile(file);
}

//=============================================================================
// Save the entity in the specified file
//=============================================================================
void Entity::writeToFile(File *file) const
{
	file->writeChar(dead);
	file->writeUnsignedInt(start);
	file->writeUnsignedInt(len);
	startPosition.writeToFile(file);
	size.writeToFile(file);
}

//=============================================================================
// Spawns the entity
//=============================================================================
void Entity::spawn()
{
	dead = false;
	position = startPosition;
}

//=============================================================================
// Kills the entity
//=============================================================================
void Entity::kill()
{
	dead = true;
}

//=============================================================================
// Sets the entity size
//=============================================================================
void Entity::setSize(const Vec2i &size)
{
	this->size = size;
}

//=============================================================================
// Sets the entity animation start and lenght
//=============================================================================
void Entity::setTexture(unsigned start, unsigned len)
{
	this->start = start;
	this->len = len;
}

//=============================================================================
// Returns true if the entity is falling
//=============================================================================
bool Entity::applyGravity()
{
	return move(GRAVITIY);
}
