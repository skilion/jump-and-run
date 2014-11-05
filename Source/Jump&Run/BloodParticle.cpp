#include <Render/Color.h>
#include <Render/Render.h>

#include "World.h"
#include "BloodParticle.h"

using namespace myengine;


const Color	BLOOD_COLOR(0.8f, 0, 0, 1);
const Vec2i	BLOOD_SIZE(2, 2);


//=============================================================================
// Constructor
//=============================================================================
BloodParticle::BloodParticle(const Vec2i &position, const Vec2f &speed, unsigned life): Particle(position, speed, life)
{
}

//=============================================================================
// Destructor
//=============================================================================
//BloodParticle::~BloodParticle() {}

//=============================================================================
// Draws the particle
//=============================================================================
void BloodParticle::draw(int pixelShift)
{
	render->setDrawColor(BLOOD_COLOR);
	render->drawFilledRect(position - Vec2i(pixelShift, 0), BLOOD_SIZE);
}

//=============================================================================
// Updates the particle
//=============================================================================
void BloodParticle::tick()
{
	if(life > 0) life--;

	Vec2i oldPosition;
	for(unsigned i = 0; i < 2; i++) {
		oldPosition = position;
		position[i] += static_cast<int>(speed[i]);

		if(world.collide(Rect(position, position))) {
			if((1 == i) && (speed[1] > 0)) { //Hit the ground
				speed.set(0, 0);
			} else {
				speed[i] = -speed[i];
			}
			position = oldPosition;
		}
	}
	
	speed[1] += 0.1f; //Simple gravity
}