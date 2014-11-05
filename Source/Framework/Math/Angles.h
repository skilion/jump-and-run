#pragma once
#ifndef __ANGLES_H__
#define __ANGLES_H__


#include "Math.h"
#include "Vector.h"


namespace myengine {
	
	class Angles
	{
	public:
			Angles();
			Angles(float pitch, float yaw, float roll);
			//~Angles();

		void	get(float &pitch, float &yaw, float &roll);
		void	getDegrees(float &pitch, float &yaw, float &roll);
		float	getPitch() const;
		float	getPitchDegrees() const;
		float	getYaw() const;
		float	getYawDegrees() const;
		float	getRoll() const;
		float	getRollDegrees() const;

		void	set(float pitch, float yaw, float roll);
		void	setDegrees(float pitch, float yaw, float roll);
		void	setPitch(float pitch);
		void	setPitchDegrees(float pitch);
		void	setYaw(float yaw);
		void	setYawDegrees(float yaw);
		void	setRoll(float roll);
		void	setRollDegrees(float roll);

		void	toVectors(Vec3f &direction, Vec3f &up, Vec3f &right) const;
		Vec3f	toDirection() const;


	private:
		float	pitch, yaw, roll;
	};

	
	//=============================================================================
	// Constructor
	//=============================================================================
	inline Angles::Angles()
	{
		pitch = yaw = roll = 0;
	}

	inline Angles::Angles(float pitch, float yaw, float roll)
	{
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = roll;
	}
	
	//=============================================================================
	// Returns the angles
	//=============================================================================
	inline void Angles::get(float &pitch, float &yaw, float &roll)
	{
		pitch = this->pitch;
		yaw = this->yaw;
		roll = this->roll;
	}

	//=============================================================================
	// Returns the angles in degrees
	//=============================================================================
	inline void Angles::getDegrees(float &pitch, float &yaw, float &roll)
	{
		pitch = this->pitch * RAD2DEG;
		yaw = this->yaw * RAD2DEG;
		roll = this->roll * RAD2DEG;
	}

	//=============================================================================
	// Retruns the pitch
	//=============================================================================
	inline float Angles::getPitch() const
	{
		return pitch;
	}

	//=============================================================================
	// Retruns the pitch in degrees
	//=============================================================================
	inline float Angles::getPitchDegrees() const
	{
		return pitch * RAD2DEG;
	}

	//=============================================================================
	// Retruns the yaw
	//=============================================================================
	inline float Angles::getYaw() const
	{
		return yaw;
	}

	//=============================================================================
	// Retruns the yaw in degrees
	//=============================================================================
	inline float Angles::getYawDegrees() const
	{
		return yaw * RAD2DEG;
	}

	//=============================================================================
	// Retruns the roll
	//=============================================================================
	inline float Angles::getRoll() const
	{
		return roll;
	}

	//=============================================================================
	// Retruns the roll in degrees
	//=============================================================================
	inline float Angles::getRollDegrees() const
	{
		return roll * RAD2DEG;
	}

	//=============================================================================
	// Sets the angles
	//=============================================================================
	inline void Angles::set(float pitch, float yaw, float roll)
	{
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = roll;
	}

	//=============================================================================
	// Sets the angles in degrees
	//=============================================================================
	inline void Angles::setDegrees(float pitch, float yaw, float roll)
	{
		this->pitch = pitch * DEG2RAD;
		this->yaw = yaw * DEG2RAD;
		this->roll = roll * DEG2RAD;
	}

	//=============================================================================
	// Sets the pitch
	//=============================================================================
	inline void Angles::setPitch(float pitch)
	{
		this->pitch = pitch;
	}

	//=============================================================================
	// Sets the pitch in degrees
	//=============================================================================
	inline void Angles::setPitchDegrees(float pitch)
	{
		this->pitch = pitch * DEG2RAD;
	}

	//=============================================================================
	// Sets the yaw
	//=============================================================================
	inline void Angles::setYaw(float yaw)
	{
		this->yaw = yaw;
	}

	//=============================================================================
	// Sets the yaw in degrees
	//=============================================================================
	inline void Angles::setYawDegrees(float yaw)
	{
		this->yaw = yaw * DEG2RAD;
	}

	//=============================================================================
	// Sets the roll
	//=============================================================================
	inline void Angles::setRoll(float roll)
	{
		this->roll = roll;
	}

	//=============================================================================
	// Sets the roll in degrees
	//=============================================================================
	inline void Angles::setRollDegrees(float roll)
	{
		this->roll = roll * DEG2RAD;
	}

}


#endif //!__ANGLES_H__