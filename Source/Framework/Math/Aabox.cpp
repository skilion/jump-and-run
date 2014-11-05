#include "Aabox.h"
#include "File/File.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Aabox::Aabox() {}

//=============================================================================
// Destructor
//=============================================================================
//Aabox::~Aabox() {}

//=============================================================================
// Returns true if the Aabox is valid
//=============================================================================
bool Aabox::isValid() const
{
	if(min.getX() >= max.getX()) return false;
	if(min.getY() >= max.getY()) return false;
	if(min.getZ() >= max.getZ()) return false;
	return true;
}

//=============================================================================
// Returns the maximum vertex relative to the specified normal
//=============================================================================
const Vec3f Aabox::getMin(const Vec3f &normal) const
{
	Vec3f min = this->min;
	if(normal.getX() < 0) min.setX(max.getX());
	if(normal.getY() < 0) min.setY(max.getY());
	if(normal.getZ() < 0) min.setZ(max.getZ());
	return min;
}

//=============================================================================
// Returns the minimum vertex relative to the specified normal
//=============================================================================
const Vec3f Aabox::getMax(const Vec3f &normal) const
{
	Vec3f max = this->max;
	if(normal.getX() < 0) max.setX(min.getX());
	if(normal.getY() < 0) max.setY(min.getY());
	if(normal.getZ() < 0) max.setZ(min.getZ());
	return max;
}

//=============================================================================
// Returns true if the specified point is in the bounding box
//=============================================================================
bool Aabox::contains(const Vec3f &point) const
{
	if(point.getX() > max.getX()) return false;
	if(point.getX() < min.getX()) return false;
	if(point.getY() > max.getY()) return false;
	if(point.getY() < min.getY()) return false;
	if(point.getZ() > max.getZ()) return false;
	if(point.getZ() < min.getZ()) return false;
	return true;
}

//=============================================================================
// Returns true if the specified the two Aabox collide
//=============================================================================
bool Aabox::intersects(const Aabox &aabox) const
{
	if(aabox.min.getX() > max.getX()) return false;
	if(aabox.max.getX() < min.getX()) return false;
	if(aabox.min.getY() > max.getY()) return false;
	if(aabox.max.getY() < min.getY()) return false;
	if(aabox.min.getZ() > max.getZ()) return false;
	if(aabox.max.getZ() < min.getZ()) return false;
	return true;
}

//=============================================================================
// Returns true if the specified ray collide with the bounding box
//=============================================================================
/*bool Aabox::intersects(const Ray &ray) const
{
	float tNear = -FLT_MAX, tFar = FLT_MAX;
	const Vec3f &origin = ray.getOrigin();
	const Vec3f &direction = ray.getDirection();

	for(int i = 0; i < 3; i++) {
		if(direction[i] == 0)	//Ray is parallel to the current axis
		{
			if((origin[i] < min[i]) || (origin[i] > max[i])) return false;
		}
		else					//Ray is not parallel to the current axis
		{
			float t1 = (min[i] - origin[i]) / direction[i];
			float t2 = (max[i] - origin[i]) / direction[i];

			if(t1 > t2) {
				float swap = t1;
				t1 = t2;
				t2 = swap;
			}

			if(t1 > tNear) tNear = t1;
			if(t2 < tFar) tFar = t2;

			if((tNear > tFar) || (tFar < 0)) return false;
		}
	}
	return true;
}*/
bool Aabox::intersects(const Ray &ray) const
{
	Vec3f intersection;
	const Vec3f &rayDirection = ray.getDirection();

	if(rayDirection.getX() < 0) {
		if(makePlane(FACE_RIGHT).intersects(ray, intersection)) {
			if(contains(intersection)) {
				return true;
			}
		}
	} else if(rayDirection.getX() > 0) {
		if(makePlane(FACE_LEFT).intersects(ray, intersection)) {
			if(contains(intersection)) {
				return true;
			}
		}
	}

	if(rayDirection.getY() < 0) {
		if(makePlane(FACE_TOP).intersects(ray, intersection)) {
			if(contains(intersection)) {
				return true;
			}
		}
	} else if(rayDirection.getY() > 0) {
		if(makePlane(FACE_BOTTOM).intersects(ray, intersection)) {
			if(contains(intersection)) {
				return true;
			}
		}
	}

	if(rayDirection.getZ() < 0) {
		if(makePlane(FACE_FRONT).intersects(ray, intersection)) {
			if(contains(intersection)) {
				return true;
			}
		}
	} else if(rayDirection.getZ() > 0) {
		if(makePlane(FACE_BEHIND).intersects(ray, intersection)) {
			if(contains(intersection)) {
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Returns true if the specified ray collide with the bounding box and gets the face interested
//=============================================================================
bool Aabox::intersects(const Ray &ray, Faces &face) const
{
	Vec3f intersection;
	const Vec3f &rayDirection = ray.getDirection();

	if(rayDirection.getX() < 0) {
		if(makePlane(FACE_RIGHT).intersects(ray, intersection)) {
			if(contains(intersection)) {
				face = FACE_RIGHT;
				return true;
			}
		}
	} else if(rayDirection.getX() > 0) {
		if(makePlane(FACE_LEFT).intersects(ray, intersection)) {
			if(contains(intersection)) {
				face = FACE_LEFT;
				return true;
			}
		}
	}

	if(rayDirection.getY() < 0) {
		if(makePlane(FACE_TOP).intersects(ray, intersection)) {
			if(contains(intersection)) {
				face = FACE_TOP;
				return true;
			}
		}
	} else if(rayDirection.getY() > 0) {
		if(makePlane(FACE_BOTTOM).intersects(ray, intersection)) {
			if(contains(intersection)) {
				face = FACE_BOTTOM;
				return true;
			}
		}
	}

	if(rayDirection.getZ() < 0) {
		if(makePlane(FACE_FRONT).intersects(ray, intersection)) {
			if(contains(intersection)) {
				face = FACE_FRONT;
				return true;
			}
		}
	} else if(rayDirection.getZ() > 0) {
		if(makePlane(FACE_BEHIND).intersects(ray, intersection)) {
			if(contains(intersection)) {
				face = FACE_BEHIND;
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Returns the plane of the specified face
//=============================================================================
const Plane Aabox::makePlane(Faces face) const
{
	Plane plane;

	switch(face)
	{
	case FACE_TOP:
		plane.make(Vec3f(0, 1, 0), max);
		break;
	case FACE_BOTTOM:
		plane.make(Vec3f(0, -1, 0), min);
		break;
	case FACE_LEFT:
		plane.make(Vec3f(-1, 0, 0), min);
		break;
	case FACE_RIGHT:
		plane.make(Vec3f(1, 0, 0), max);
		break;
	case FACE_FRONT:
		plane.make(Vec3f(0, 0, 1), max);
		break;
	case FACE_BEHIND:
		plane.make(Vec3f(0, 0, -1), min);
		break;
	}

	return plane;
}

//=============================================================================
// Reads the vector from a file
//=============================================================================
void Aabox::readFromFile(File *file)
{
	min.readFromFile(file);
	max.readFromFile(file);
}

//=============================================================================
// Writes the vector to a file
//=============================================================================
void Aabox::writeToFile(File *file) const
{
	min.writeToFile(file);
	max.writeToFile(file);
}