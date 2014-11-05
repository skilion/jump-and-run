#pragma once
#ifndef __PARTICLE_H__
#define __PARTICLE_H__


#include <Math/Vector.h>

using namespace myengine;


class Particle
{
public:
		Particle(const Vec2i &position, const Vec2f &speed, unsigned life);
		virtual ~Particle();

	virtual void	draw(int pixelShift) = 0;
	virtual void	tick() = 0;
	
	bool			isExpired();

	const Vec2i		&getPosition();
	void			setPosition(const Vec2i &position);
	

protected:
	Vec2i		position;
	Vec2f		speed;
	unsigned	life;
};


#endif //!__PARTICLE_H__