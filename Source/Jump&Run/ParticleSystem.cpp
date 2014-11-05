#include "BloodParticle.h"
#include "Random.h"
#include "ParticleSystem.h"

#include "Math/Math.h"

using namespace myengine;


ParticleSystem particleSystem;

const unsigned	EFFECT_BLOOD_AREA			= 10;
const unsigned	EFFECT_BLOOD_PARTICLE_NO	= 100;
const float		EFFECT_BLOOD_PARTICLE_SPEED	= 3;


//=============================================================================
// Constructor
//=============================================================================
ParticleSystem::ParticleSystem()
{
	count = 0;
}

//=============================================================================
// Destructor
//=============================================================================
ParticleSystem::~ParticleSystem()
{
	reset();
}

//=============================================================================
// Creates an effect
//=============================================================================
void ParticleSystem::createEffect(ParticleSystem::Effect effect, const Vec2i &position)
{
	switch(effect)
	{
	case EFFECT_BLOOD_DROP:
	case EFFECT_BLOOD_SPLASH:
		particles.ensureSize(count + EFFECT_BLOOD_PARTICLE_NO);
		for(unsigned i = 0; i < EFFECT_BLOOD_PARTICLE_NO; i++)
		{
			//Particle speed
			Vec2f speed(random.getFloat(), random.getFloat());
			speed *= EFFECT_BLOOD_PARTICLE_SPEED;
			if(random.getBool()) speed[0] = -speed[0];

			if(EFFECT_BLOOD_SPLASH == effect) if(random.getBool()) speed[1] = -speed[1] * 2;

			//Particle distance from position
			Vec2i dist(random.getInt(EFFECT_BLOOD_AREA), random.getInt(EFFECT_BLOOD_AREA));
			if(random.getBool()) dist[0] = -dist[0];
			if(random.getBool()) dist[1] = -dist[1];

			particles[count + i] = new BloodParticle(position + dist, speed, 300);
		}
		count += EFFECT_BLOOD_PARTICLE_NO;
		break;
	}
}

//=============================================================================
// Deletes all particles
//=============================================================================
void ParticleSystem::reset()
{
	for(unsigned i = 0; i < count; i++) {
		delete particles[i];
	}
	count = 0;
}

//=============================================================================
// Draws all particles
//=============================================================================
void ParticleSystem::draw(int pixelShift)
{
	for(unsigned i = 0; i < count; i++) {
		Particle *particle = particles[i];
		if(!particle->isExpired()) {
			particle->draw(pixelShift);
		}
	}
}

//=============================================================================
// Update all particles
//=============================================================================
void ParticleSystem::tick()
{
	bool empty = true;
	for(unsigned i = 0; i < count; i++) {
		Particle *particle = particles[i];
		if(!particle->isExpired()) {
			empty = false;
			particle->tick();
		} else {
			delete particle;
			particles[i] = particles[count - 1];
			count--;
		}
	}
}