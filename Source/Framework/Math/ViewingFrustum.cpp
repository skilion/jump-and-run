#include "Math.h"
#include "Aabox.h"
#include "Sphere.h"
#include "ViewingFrustum.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
//ViewingFrustum::ViewingFrustum() {}

//=============================================================================
// Destructor
//=============================================================================
//ViewingFrustum::~ViewingFrustum() {}

//=============================================================================
// Extract the planes of the viewing frustum
//=============================================================================
void ViewingFrustum::make(const Vec3f &position,  const Vec3f &direction, const Vec3f &up, const Vec3f &right, float fovy, float aspect, float near, float far)
{
	float tang = tan(fovy * (DEG2RAD / 2));

	//Near plane points
	
	float nearHeight = tang * near;
	float nearWidth = nearHeight * aspect;
	
	Vec3f nearCenter = position + direction * near;
	Vec3f nearUp = up * nearHeight;
	Vec3f nearRight = right * nearWidth;

	Vec3f nearTopLeft = nearCenter + nearUp - nearRight;
	Vec3f nearTopRight = nearCenter + nearUp + nearRight;
	Vec3f nearBottomLeft = nearCenter - nearUp - nearRight;
	Vec3f nearBottomRight = nearCenter - nearUp + nearRight;

	//Far plane points
	
	float farHeight = tang * far;
	float farWidth = farHeight * aspect;

	Vec3f farCenter = position + direction * far;
	Vec3f farUp = up * farHeight;
	Vec3f farRight = right * farWidth;

	//Vec3f farTopLeft = farCenter + farUp - farRight;
	Vec3f farTopRight = farCenter + farUp + farRight;
	Vec3f farBottomLeft = farCenter - farUp - farRight;
	Vec3f farBottomRight = farCenter - farUp + farRight;

	//Left plane
	planes[0].make(nearTopLeft, nearBottomLeft, farBottomLeft);

	//Right plane
	planes[1].make(farBottomRight, nearBottomRight, nearTopRight);

	//Top plane
	planes[2].make(farTopRight, nearTopRight, nearTopLeft);

	//Bottom plane
	planes[3].make(nearBottomLeft, nearBottomRight, farBottomRight);

	//Near plane
	planes[4].make(direction, nearCenter);

	//Far plane
	planes[5].make(-direction, farCenter);
}

//=============================================================================
// Test if the viewing frustum contains the specified point
//=============================================================================
ViewingFrustum::TestResult ViewingFrustum::test(const Vec3f &point) const
{
	for(unsigned i = 0; i < 6; i++) {
		if(planes[i].distance(point) < 0)
			return OUTSIDE;
	}

	return INSIDE;
}

//=============================================================================
// Test if the viewing frustum contains the specified Sphere
//=============================================================================
ViewingFrustum::TestResult ViewingFrustum::test(const Sphere &sphere) const
{
	TestResult result = INSIDE;

	float distance;
	for(unsigned i = 0; i < 6; i++) {
		distance = planes[i].distance(sphere.getCenter());
		if(distance < -sphere.getRadius()) return OUTSIDE;
		else if(distance < sphere.getRadius()) result = INTERSECT;
	}

	return result;
}

//=============================================================================
// Test if the viewing frustum contains the specified Aabox
//=============================================================================
ViewingFrustum::TestResult ViewingFrustum::test(const Aabox &aabox) const
{
	TestResult result = INSIDE;
	
	for(unsigned i = 0; i < 6; i++) {
		if(planes[i].distance(aabox.getMax(planes[i].getNormal())) < 0)
			return OUTSIDE;
		else if (planes[i].distance(aabox.getMin(planes[i].getNormal())) < 0)
			result = INTERSECT;
	}

	return result;
}