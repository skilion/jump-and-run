#pragma once
#ifndef __AABOX_H__
#define __AABOX_H__


#include "Ray.h"
#include "Plane.h"
#include "Sphere.h"
#include "Vector.h"


namespace myengine {

	class File;

	class Aabox //Axis-aligned box
	{
	public:
			Aabox();
			Aabox(const Vec3f &min, const Vec3f &max);
			//~Aabox();


		enum Faces {
			FACE_TOP,
			FACE_BOTTOM,
			FACE_LEFT,
			FACE_RIGHT,
			FACE_FRONT,
			FACE_BEHIND
		};
			
		bool			isValid() const;
		
		void			get(Vec3f &min, Vec3f &max);
		const Vec3f		&getMin() const;
		const Vec3f		&getMax() const;
		const Vec3f		getMin(const Vec3f &normal) const;
		const Vec3f		getMax(const Vec3f &normal) const;

		void			set(const Vec3f &min, const Vec3f &max);
		void			setMin(const Vec3f &min);
		void			setMax(const Vec3f &max);
		
		bool			contains(const Vec3f &point) const;
		bool			intersects(const Aabox &aabox) const;
		bool			intersects(const Ray &ray) const;
		bool			intersects(const Ray &ray, Faces &face) const;
		//bool			intersects(const Ray &ray, Faces &face, Vec3f &intesection) const;

		void			readFromFile(File *file);
		void			writeToFile(File *file) const;


	private:
		const Plane		makePlane(Faces face) const;

		Vec3f			min, max;
	};


	//============================================================================
	// Constructor
	//============================================================================
	inline Aabox::Aabox(const Vec3f &min, const Vec3f &max)
	{
		this->min = min;
		this->max = max;
	}

	//=============================================================================
	// Gets the bounds of the Aabox
	//=============================================================================
	inline void Aabox::get(Vec3f &min, Vec3f &max)
	{
		min = this->min;
		max = this->max;
	}

	//=============================================================================
	// Returns the minimum vertex of the Aabox
	//=============================================================================
	inline const Vec3f &Aabox::getMin() const
	{
		return min;
	}

	//=============================================================================
	// Returns the maximum vertex of the Aabox
	//=============================================================================
	inline const Vec3f &Aabox::getMax() const
	{
		return max;
	}

	//=============================================================================
	// Sets the bounds of the Aabox
	//=============================================================================
	inline void Aabox::set(const Vec3f &min, const Vec3f &max)
	{
		this->min = min;
		this->max = max;
	}

	//=============================================================================
	// Sets the minimum vertex of the Aabox
	//=============================================================================
	inline void Aabox::setMin(const Vec3f &min)
	{
		this->min = min;
	}

	//============================================================================
	// Sets the maximum vertex of the Aabox
	//=============================================================================
	inline void Aabox::setMax(const Vec3f &max)
	{
		this->max = max;
	}
	
}


#endif //!__AABOX_H__