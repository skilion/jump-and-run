#pragma once
#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__


#include <Containers/Array.h>

#include "Particle.h"

using namespace myengine;


class ParticleSystem
{
public:
	enum Effect {
		EFFECT_BLOOD_SPLASH,
		EFFECT_BLOOD_DROP,
	};

		ParticleSystem();
		~ParticleSystem();

	void	createEffect(Effect effect, const Vec2i &position);
	void	reset();

	void	draw(int pixelShift);
	void	tick();


private:
	unsigned			count;
	Array<Particle*>	particles;
};


extern ParticleSystem particleSystem;


#endif //!__PARTICLE_SYSTEM_H__