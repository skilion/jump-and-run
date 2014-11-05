#pragma once
#ifndef __OBJECT_H__
#define __OBJECT_H__


#include "Math/Aabox.h"
#include "Math/Vector3.h"


namespace myengine {
	
	class Object
	{
	public:
			//Object();
			//~Object();
			
		const Aabox		&getAabox() const;
		float			getMass() const;
		
		const Aabox		&setAabox(const Aabox &aabox) const;
		void			setMass(float mass) const;
		
		void			tick(float time);
		void			applyForce(Vec3f force);
		

	private:
		float	mass;
		Vec3f	force, velocity;
		
		Aabox	aabox;
	};
	

}


#endif //!__TEMPLATE_H__