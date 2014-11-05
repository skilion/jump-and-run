#pragma once
#ifndef __PLANE_H__
#define __PLANE_H__


#include "Vector.h"


namespace myengine {

	class Ray;
	class Sphere;
	
	class Plane
	{
	public:
			Plane();
			Plane(float a, float b, float c, float d);
			Plane(const Vec3f &normal, const Vec3f &point);
			Plane(const Vec3f &point1, const Vec3f &point2, const Vec3f &point3);
			//~Plane();
			
		bool			isValid() const;
			
		const Vec3f		getNormal() const;
		
		void			set(float a, float b, float c, float d);

		void			make(const Vec3f &normal, const Vec3f &point);
		void			make(const Vec3f &point1, const Vec3f &point2, const Vec3f &point3);
		
		void			normalize();
		
		float			distance(const Vec3f &point) const;
		bool			intersects(const Vec3f &point) const;
		bool			intersects(const Ray &ray) const;
		bool			intersects(const Ray &ray, Vec3f &intersection) const;
		

	private:
		float	a, b, c, d;
	};
	
	
	//=========================================================================
	// Constructor
	//=========================================================================
	inline Plane::Plane()
	{
		a = b = c = d = 0;
	}

	inline Plane::Plane(float a, float b, float c, float d)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}

	inline Plane::Plane(const Vec3f &normal, const Vec3f &point)
	{
		make(normal, point);
	}
	
	inline Plane::Plane(const Vec3f &point1, const Vec3f &point2, const Vec3f &point3)
	{
		make(point1, point2, point3);
	}
	
	//=========================================================================
	// Returns the normal of the plane
	//=========================================================================
	inline const Vec3f Plane::getNormal() const
	{
		return Vec3f(a, b, c);
	}
	
	//=========================================================================
	// Sets the components of the plane
	//=========================================================================
	inline void Plane::set(float a, float b, float c, float d)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		//normalize();
	}

}


#endif //!__PLANE_H__