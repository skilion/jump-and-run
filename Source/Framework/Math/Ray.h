#pragma once
#ifndef __RAY_H__
#define __RAY_H__


#include "Vector.h"


namespace myengine {
	
	class Ray
	{
	public:
			Ray();
			Ray(const Vec3f &origin, const Vec3f &direction);
			//~Ray();
			
		bool			isValid() const;
			
		void			get(Vec3f &origin, Vec3f &direction) const;
		const Vec3f		&getOrigin() const;
		const Vec3f		&getDirection() const;
		
		void			set(const Vec3f &origin, const Vec3f &direction);
		void			setOrigin(const Vec3f &origin);
		void			setDirection(const Vec3f &direction);
		

	private:
		Vec3f	origin, direction;
	};
	
	
	//=========================================================================
	// Constructor
	//=========================================================================
	inline Ray::Ray(const Vec3f &origin, const Vec3f &direction)
	{
		this->origin = origin;
		this->direction = direction;
	}
	
	//=========================================================================
	// Gets the components of the ray
	//=========================================================================
	inline void Ray::get(Vec3f &origin, Vec3f &direction) const
	{
		origin = this->origin;
		direction = this->direction;
	}
	
	//=========================================================================
	// Returns the origin of the ray
	//=========================================================================
	inline const Vec3f &Ray::getOrigin() const
	{
		return origin;
	}
	
	//=========================================================================
	// Returns the direction of the ray
	//=========================================================================
	inline const Vec3f &Ray::getDirection() const
	{
		return direction;
	}
	
	//=========================================================================
	// Sets the components of the ray
	//=========================================================================
	inline void Ray::set(const Vec3f &origin, const Vec3f &direction)
	{
		this->origin = origin;
		this->direction = direction;
	}
	
	//=========================================================================
	// Sets the origin of the ray
	//=========================================================================
	inline void Ray::setOrigin(const Vec3f &origin)
	{
		this->origin = origin;
	}
	
	//=========================================================================
	// Sets the direction of the ray
	//=========================================================================
	inline void Ray::setDirection(const Vec3f &direction)
	{
		this->direction = direction;
	}

}


#endif //!__RAY_H__