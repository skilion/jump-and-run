#pragma once
#ifndef __SPHERE_H__
#define __SPHERE_H__


#include "Vector.h"


namespace myengine {
	
	class Sphere
	{
	public:
			Sphere();
			Sphere(const Vec3f &center, float radius);
			//~Sphere();
			
		bool			isValid() const;
			
		void			get(Vec3f &center, float &radius) const;
		const Vec3f		&getCenter() const;
		float			getRadius() const;
		
		void			set(const Vec3f &center, float radius);
		void			setCenter(const Vec3f &center);
		void			setRadius(float radius);

		bool			contains(const Vec3f &point) const;
		bool			collides(const Sphere &sphere) const;
		

	private:
		Vec3f	center;
		float	radius;
	};
	
	
	//=========================================================================
	// Constructor
	//=========================================================================
	inline Sphere::Sphere(const Vec3f &center, float radius)
	{
		this->center = center;
		this->radius = radius;
	}
	
	//=========================================================================
	// Gets the components of the ray
	//=========================================================================
	inline void Sphere::get(Vec3f &center, float &radius) const
	{
		center = this->center;
		radius = this->radius;
	}
	
	//=========================================================================
	// Returns the center of the sphere
	//=========================================================================
	inline const Vec3f &Sphere::getCenter() const
	{
		return center;
	}
	
	//=========================================================================
	// Returns the radius of the sphere
	//=========================================================================
	inline float Sphere::getRadius() const
	{
		return radius;
	}
	
	//=========================================================================
	// Sets the components of the sphere
	//=========================================================================
	inline void Sphere::set(const Vec3f &center, float radius)
	{
		this->center = center;
		this->radius = radius;
	}
	
	//=========================================================================
	// Sets the center of the sphere
	//=========================================================================
	inline void Sphere::setCenter(const Vec3f &center)
	{
		this->center = center;
	}
	
	//=========================================================================
	// Sets the radius of the sphere
	//=========================================================================
	inline void Sphere::setRadius(float radius)
	{
		this->radius = radius;
	}

}


#endif //!__SPHERE_H__