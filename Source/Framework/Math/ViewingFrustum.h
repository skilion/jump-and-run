#pragma once
#ifndef __VIEWING_FRUSTUM_H__
#define __VIEWING_FRUSTUM_H__


//=============================================================================
//
//		Class for viewing frustum culling (http://www.lighthouse3d.com/tutorials/view-frustum-culling)
//
//=============================================================================


#include "Plane.h"
#include "Angles.h"
#include "Vector.h"


namespace myengine {

	
	class ViewingFrustum
	{
	public:
		enum TestResult {
			OUTSIDE,
			INSIDE,
			INTERSECT
		};

			//ViewingFrustum();
			//ViewingFrustum(const Vec3f &position, const Vec3f &direction, float near, float far);
			//~ViewingFrustum();
		
		void		make(const Vec3f &position,  const Vec3f &direction, const Vec3f &up, const Vec3f &right, float fovy, float aspect, float near, float far);
		
		TestResult	test(const Vec3f &point) const;
		TestResult	test(const Sphere &sphere) const;
		TestResult	test(const Aabox &aabox) const;

	private:
		/*enum Planes {
			PLANE_LEFT,
			PLANE_RIGHT,
			PLANE_TOP,
			PLANE_BOTTOM,
			PLANE_NEAR,
			PLANE_FAR,
		};*/

		Plane		planes[6];
	};

	
	/*//=============================================================================
	// Constructor
	//=============================================================================
	inline ViewingFrustum::ViewingFrustum(float fovy, float aspect, float near, float far)
	{
		make(fovy, aspect, near, far);
	}*/

}


#endif //!__VIEWING_FRUSTUM_H__