#include "Line.h"
#include "Math.h"
#include "Vector.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Line::Line()
{
	a = b = c = 0;
}

//=============================================================================
// Destructor
//=============================================================================
//Line::~Line() {}

//=========================================================================
// Returns true if the line is valid
//=========================================================================
bool Line::isValid() const
{
	return (a != 0) || (b != 0);
}

//=========================================================================
// Makes the line given two points
//=========================================================================
void Line::make(const Vec2f &p1, const Vec2f &p2)
{
	float x1, y1, x2, y2;
	p1.get(x1, y1);
	p2.get(x2, y2);

	a = y1 - y2;
	b = x2 - x1;
	c = x1 * (y2 - y1) + y1 * (x1 - x2);
}


//=========================================================================
// Returns true if the line intersects and the intersection
//=========================================================================
Line::LineLineIntersection Line::intersects(const Line &line, Vec2f &intersection)
{
	float a2 = line.a, b2 = line.b, c2 = line.c;


	float d = a * b2 - b * 2;

	if(fzero(d)) {
		if(a2 * c - a * c2) return COINCIDENT;
		return PARALLEL;
	}
	intersection.setX((a2 * c - a * c2) / d);
	intersection.setY((a2 * c - a * c2) / d);
	return INCIDENT;
}