#include "Angles.h"

using namespace myengine;


//=============================================================================
// Destructor
//=============================================================================
//Angles::~Angles() {}

//=============================================================================
// Converts the angles in three vectors
//=============================================================================
void Angles::toVectors(Vec3f &direction, Vec3f &up, Vec3f &right) const
{
	float sinPitch, cosPitch, sinYaw, cosYaw, sinRoll, cosRoll;

	sincos(pitch, sinPitch, cosPitch);
	sincos(yaw, sinYaw, cosYaw);
	sincos(roll, sinRoll, cosRoll);

	direction.set(sinRoll * sinPitch + cosRoll * sinYaw * cosPitch,
		-cosRoll * sinPitch + sinRoll * sinYaw * cosPitch,
		cosYaw * cosPitch);
	direction.normalize();

	up.set(-sinRoll * cosPitch + cosRoll * sinYaw * sinPitch,
		cosRoll * cosPitch + sinRoll * sinYaw * sinPitch,
		cosYaw * sinPitch);
	up.normalize();

	//right = direction.cross(up);
	right.set(cosRoll * cosYaw,
		sinRoll * cosYaw,
		-sinYaw);
	right.normalize();
}

//=============================================================================
// Returns the direction vector
//=============================================================================
Vec3f Angles::toDirection() const
{
	float sinPitch, cosPitch, sinYaw, cosYaw, sinRoll, cosRoll;

	sincos(pitch, sinPitch, cosPitch);
	sincos(yaw, sinYaw, cosYaw);
	sincos(roll, sinRoll, cosRoll);

	Vec3f direction(sinRoll * sinPitch + cosRoll * sinYaw * cosPitch,
		-cosRoll * sinPitch + sinRoll * sinYaw * cosPitch,
		cosYaw * cosPitch);
	direction.normalize();

	return direction;
}