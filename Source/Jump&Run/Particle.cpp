#include "Particle.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Particle::Particle(const Vec2i &position, const Vec2f &speed, unsigned life)
{
	this->position = position;
	this->speed = speed;
	this->life = life;
}

//=============================================================================
// Destructor
//=============================================================================
Particle::~Particle()
{
}

//=============================================================================
// Returns true if the particle is expired
//=============================================================================
bool Particle::isExpired()
{
	return 0 == life;
}

//=============================================================================
// Returns the particle position
//=============================================================================
const Vec2i &Particle::getPosition()
{
	return position;
}

//=============================================================================
// Sets the particle position
//=============================================================================
void Particle::setPosition(const Vec2i &position)
{
	this->position = position;
}