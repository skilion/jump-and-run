#include "Ray.h"
#include "Math.h"
#include "Plane.h"

using namespace myengine;


//=============================================================================
// Destructor
//=============================================================================
//Plane::~Plane() {}

//=========================================================================
// Returns true if the ray is valid
//=========================================================================
bool Plane::isValid() const
{
	//if(!normal.isValid()) return false;
	//if(!d.isValid()) return false;
	return !(fzero(a) && fzero(b) && fzero(a));
}

//=========================================================================
// Makes the plane using a normal vector and a point
//=========================================================================
void Plane::make(const Vec3f &normal, const Vec3f &point)
{
	a = normal.getX();
	b = normal.getY();
	c = normal.getZ();
	d = -normal.dot(point);
}

//=========================================================================
// Normalize the normal of the plane
//=========================================================================
void Plane::normalize()
{
	float mag = sqrt(square<float>(a) + square<float>(b) + square<float>(c));
	a /= mag;
	b /= mag;
	c /= mag;
}

//=========================================================================
// Makes the plane using three points (given in counter-clockwise order)
//=========================================================================
void Plane::make(const Vec3f &point1, const Vec3f &point2, const Vec3f &point3)
{
	Vec3f normal = (point1 - point2).cross(point2 - point3);
	normal.normalize();
	make(normal, point1);
}
//=========================================================================
// Returns the distance between the point and the plane
//=========================================================================
float Plane::distance(const Vec3f &point) const
{
	return (a * point.getX() + b * point.getY() + c * point.getZ() + d);
}

//=========================================================================
// Returns true if the specified point lies in the plane
//=========================================================================
bool Plane::intersects(const Vec3f &point) const
{
	return fzero(a * point.getX() + b *point.getY() + c * point.getZ() + d);
}

//=========================================================================
// Returns true if the ray intersects within the plane
//=========================================================================
bool Plane::intersects(const Ray &ray) const
{
	const Vec3f &rayOrigin = ray.getOrigin();
	const Vec3f &rayDir = ray.getDirection();

	float angle = a * rayDir.getX() + b * rayDir.getY() + c * rayDir.getZ();

	if(fzero(angle)) { //ray parallel to the plane
		return intersects(ray.getOrigin());
	} else if(angle < 0) {
		float t = -(a * rayOrigin.getX() + b * rayOrigin.getY() + c * rayOrigin.getZ() + d) / angle;

		//if (t < 0) the ray intersects the plane behind origin
		if(t >= 0) return true;
	}
	//if (angle > 0) the ray intersects the plane on the hidden face

	return false;
}

//=========================================================================
// Returns true if the ray intersects within the plane and gets the point of intersection
//=========================================================================
bool Plane::intersects(const Ray &ray, Vec3f &intersection) const
{
	const Vec3f &rayOrigin = ray.getOrigin();
	const Vec3f &rayDir = ray.getDirection();

	float angle = a * rayDir.getX() + b * rayDir.getY() + c * rayDir.getZ();

	if(fzero(angle)) {
		if(intersects(rayOrigin)) {
			//intersection = point; TODO!
			return true;
		}
	} else if(angle < 0) {
		float t = -(a * rayOrigin.getX() + b * rayOrigin.getY() + c * rayOrigin.getZ() + d) / angle;

		if(t >= 0) {
			intersection.setX(rayOrigin.getX() + rayDir.getX() * t);
			intersection.setY(rayOrigin.getY() + rayDir.getY() * t);
			intersection.setZ(rayOrigin.getZ() + rayDir.getZ() * t);
			return true;
		}
	}

	return false;
}