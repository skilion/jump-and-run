#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "Math/Angles.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"


namespace myengine {
	
	class Camera
	{
	public:
			Camera();
			Camera(const Vec3f &position, const Angles &angles, float fov = 90);
			//~Camera();

		const Vec3f		&getPosition() const;
		const Angles	&getAngles() const;
		float			getFov() const;
		
		void			setPosition(const Vec3f &position);
		void			setAngles(const Angles &angles);
		//void			setRotationMatrix(const Matrix4f &matrix);
		void			setFov(float fov);

		void			rotateX(float pitch);
		void			rotateY(float yaw);
		void			rotateZ(float roll);
		
		const Matrix4f	computeRotationMatrix() const;


	protected:
		Vec3f		position;
		Angles		angles;
		//Matrix4f	rotation;
		float		fov;
	};


	//=========================================================================
	// Returns the camera position
	//=========================================================================
	inline const Vec3f &Camera::getPosition() const
	{
		return position;
	}
	
	//=========================================================================
	// Sets the camera angles
	//=========================================================================
	inline const Angles &Camera::getAngles() const
	{
		return angles;
	}

	//=========================================================================
	// Returns the camera fov
	//=========================================================================
	inline float Camera::getFov() const
	{
		return fov;
	}
	
	//=========================================================================
	// Sets the camera position
	//=========================================================================
	inline void Camera::setPosition(const Vec3f &position)
	{
		this->position = position;
	}
	
	//=========================================================================
	// Sets the camera angles
	//=========================================================================
	inline void Camera::setAngles(const Angles &angles)
	{
		this->angles = angles;
	}
	
	//=========================================================================
	// Sets the camera fov
	//=========================================================================
	inline void Camera::setFov(float fov)
	{
		this->fov = fov;
	}

	//=========================================================================
	// Returns the rotation matrix
	//=========================================================================
	inline const Matrix4f Camera::computeRotationMatrix() const
	{
		return (
			Matrix4f::makeRotationXMatrix(angles.getPitch()) *
			Matrix4f::makeRotationYMatrix(angles.getYaw()) *
			Matrix4f::makeRotationZMatrix(angles.getRoll())
		);
	}
	
}


#endif //!__CAMERA_H__