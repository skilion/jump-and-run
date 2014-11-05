#include "Rect.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Rect::Rect() {}

Rect::Rect(const Vec2i &min, const Vec2i &max)
{
	this->min = min;
	this->max = max;
}

//=============================================================================
// Destructor
//=============================================================================
//template<class T>
//Rect::~Rect() {}

//=============================================================================
// Returns true if the rects are equal
//=============================================================================
bool Rect::isEqual(const Rect &rect) const
{
	return (min == rect.min && max == rect.max) ? true : false;
}

//=============================================================================
// Returns true if the rectangle contains the specified point
//=============================================================================
bool Rect::contains(const Vec2i &point) const
{
	if(point.getX() > max.getX()) return false;
	if(point.getX() < min.getX()) return false;
	if(point.getY() > max.getY()) return false;
	if(point.getY() < min.getY()) return false;
	return true;
}

//=============================================================================
// Returns true if the rectangles intersect
//=============================================================================
bool Rect::intersects(const Rect &rect) const
{
	if(min.getX() > rect.max.getX()) return false;
	if(max.getX() < rect.min.getX()) return false;
	if(min.getY() > rect.max.getY()) return false;
	if(max.getY() < rect.min.getY()) return false;
	return true;
}