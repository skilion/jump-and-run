#include "Math.h"
#include "Sphere.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Sphere::Sphere() {}

//=============================================================================
// Destructor
//=============================================================================
//Sphere::~Sphere() {}

//=========================================================================
// Returns true if the ray is valid
//=========================================================================
bool Sphere::isValid() const
{
	return radius > 0;
}

//=========================================================================
// Returns true if the sphere contains the point
//=========================================================================
bool Sphere::contains(const Vec3f &point) const
{
	return center.distance(point) <= radius;
}

//=========================================================================
// Returns true if the spheres collide
//=========================================================================
bool Sphere::collides(const Sphere &sphere) const
{
	return center.distance(sphere.center) <= (radius + sphere.radius);
}