#pragma once
#ifndef __LINE_H__
#define __LINE_H__


namespace myengine {

	template<unsigned SIZE, class T> class Vector;
	typedef Vector<2, float> Vec2f;
	
	class Line
	{
	public:
		enum LineLineIntersection {
			INCIDENT,
			PARALLEL,
			COINCIDENT
		};

			Line();
			//Line(float m, float b);
			Line(const Vec2f &p1, const Vec2f &p2);
			//~Line();
			
		bool	isValid() const;
		
		//void	get(float &m, float &b) const;
		//float	getSlope() const;
		//float	getYIntercept() const;
		
		//void	set(float m, float b);
		//void	setSlope(float m);
		//void	setYIntercept(float b);

		void	make(const Vec2f &p1, const Vec2f &p2);
		
		
		LineLineIntersection	intersects(const Line &line, Vec2f &intersection);


	private:
		float	a, b, c;	//ax + by + c = 0
	};
	
	
	//=========================================================================
	// Constructor
	//=========================================================================
	/*inline Line::Line(float m, float b)
	{
		this->m = m;
		this->b = b;
	}*/

	inline Line::Line(const Vec2f &p1, const Vec2f &p2)
	{
		make(p1, p2);
	}
	
	//=========================================================================
	// Gets the components of the line
	//=========================================================================
	/*inline void Line::get(float &m, float &b) const
	{
		m = this->m;
		b = this->b;
	}
	
	//=========================================================================
	// Returns the slope of the line
	//=========================================================================
	inline float Line::getSlope() const
	{
		return m;
	}
	
	//=========================================================================
	// Returns the Y-itercept of the line
	//=========================================================================
	inline float Line::getYIntercept() const
	{
		return b;
	}
	
	//=========================================================================
	// Sets the components of the line
	//=========================================================================
	inline void Line::set(float m, float q)
	{
		this->m = m;
		this->b = b;
	}
	
	//=========================================================================
	// Sets the slope of the line
	//=========================================================================
	inline void Line::setSlope(float m)
	{
		this->m = m;
	}
	
	//=========================================================================
	// Sets the Y-itercept of the line
	//=========================================================================
	inline void Line::setYIntercept(float b)
	{
		this->b = b;
	}*/

}


#endif //!__LINE_H__