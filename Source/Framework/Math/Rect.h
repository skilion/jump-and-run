#pragma once
#ifndef __RECT_H__
#define __RECT_H__


#include "Vector.h"


namespace myengine {

	class Rect
	{
	public:
			Rect();
			Rect(const Vec2i &min, const Vec2i &max);
			//~Rect();

		bool			operator==(const Rect &rect) const;
		bool			operator!=(const Rect &rect) const;

		bool			isEqual(const Rect &rect) const;

		void			get(Vec2i &min, Vec2i &max) const;
		const Vec2i		&getMin() const;
		const Vec2i		&getMax() const;
		void			getSize(Vec2i &size) const;

		void			set(const Vec2i &min, const Vec2i &max);
		void			setMin(const Vec2i &min);
		void			setMax(const Vec2i &max);
		void			setSize(const Vec2i &size);

		bool			contains(const Vec2i &point) const;
		bool			intersects(const Rect &rect) const;


	private:
		Vec2i	min, max;
	};

	
	//=========================================================================
	// Overload of operator==
	//=========================================================================
	inline bool Rect::operator==(const Rect &rect) const
	{
		return isEqual(rect);
	}

	//=========================================================================
	// Overload of operator!=
	//=========================================================================
	inline bool Rect::operator!=(const Rect &rect) const
	{
		return !isEqual(rect);
	}
	
	//=========================================================================
	// Returns the vectors of the rectangle
	//=========================================================================
	inline void Rect::get(Vec2i &min, Vec2i &max) const
	{
		min = this->min;
		max = this->max;
	}

	//=========================================================================
	// Returns the min vector of the rectangle
	//=========================================================================
	inline const Vec2i &Rect::getMin() const
	{
		return min;
	}

	//=========================================================================
	// Returns the max vector of the rectangle
	//=========================================================================
	inline const Vec2i &Rect::getMax() const
	{
		return max;
	}
	
	//=========================================================================
	// Returns the size of the rectangle
	//=========================================================================
	inline void Rect::getSize(Vec2i &size) const
	{
		size = min + max - min;
	}

	//=========================================================================
	// Sets the vectors of the rectangle
	//=========================================================================
	inline void Rect::set(const Vec2i &min, const Vec2i &max)
	{
		this->min = min;
		this->max = max;
	}

	//=========================================================================
	// Sets the min vector of the rectangle
	//=========================================================================
	inline void Rect::setMin(const Vec2i &min)
	{
		this->min = min;
	}

	//=========================================================================
	// Sets the max vector of the rectangle
	//=========================================================================
	inline void Rect::setMax(const Vec2i &max)
	{
		this->max = max;
	}


	//=========================================================================
	// Sets the size of the rectangle
	//=========================================================================
	inline void Rect::setSize(const Vec2i &size)
	{
		max = min + size;
	}

}


#endif //!__RECT_H__
