#pragma once
#ifndef __VOXEL_H__
#define __VOXEL_H__


#include "Render/Color.h"


namespace myengine {
		
	class Voxel
	{
	public:
			//Voxel();
			//~Voxel();

		bool			operator==(const Voxel &voxel) const;
		bool			operator!=(const Voxel &voxel) const;

		bool			isTransparent() const;
		bool			compare(const Voxel &voxel) const;

			const Color	&getColor() const;

			void			setColor(const Color &color);

		void			readFromFile(File *file);
		void			writeToFile(File *file) const;


	private:
		Color	color;
	};


	//=========================================================================
	// Overlad of operator==
	//=========================================================================
	inline bool Voxel::operator==(const Voxel &voxel) const
	{
		return compare(voxel);
	}

	//=========================================================================
	// Overlad of operator!=
	//=========================================================================
	inline bool Voxel::operator!=(const Voxel &voxel) const
	{
		return !compare(voxel);
	}
	
	//=========================================================================
	// Returns true if the voxel is empty
	//=========================================================================
	inline bool Voxel::isTransparent() const
	{
		return color.getAlpha() < 1;
	}
	
	//=========================================================================
	// Returns the color of the voxel
	//=========================================================================
	inline const Color &Voxel::getColor() const
	{
		return color;
	}
	
	//=============================================================================
	// Sets the voxel color
	//=============================================================================
	inline void Voxel::setColor(const Color &color)
	{
		this->color = color;
	}

}


#endif //!__VOXEL_H__