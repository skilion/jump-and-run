#pragma once
#ifndef __BLOOD_H__
#define __BLOOD_H__


#include "Particle.h"


class BloodParticle: public Particle
{
public:
		BloodParticle(const Vec2i &position, const Vec2f &speed, unsigned life);
		//virtual ~BloodParticle();

	virtual void	draw(int pixelShift);
	virtual void	tick();
};


#endif //!__BLOOD_H__