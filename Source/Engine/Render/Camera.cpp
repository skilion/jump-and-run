#include "Camera.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Camera::Camera()
{
	fov = 90;
}

Camera::Camera(const Vec3f &position, const Angles &angles, float fov)
{
	this->position = position;
	this->angles = angles;
	this->fov = fov;
}

//=============================================================================
// Destructor
//=============================================================================
//Camera::~Camera() {}

//=============================================================================
// Rotates the camera on the X axis
//=============================================================================
void Camera::rotateX(float pitch)
{
	//rotation *= Matrix4f::makeRotationXMatrix(pitch);
	angles.setPitch(angles.getPitch() + pitch);
}

//=============================================================================
// Rotates the camera on the Y axis
//=============================================================================
void Camera::rotateY(float yaw)
{
	//rotation *= Matrix4f::makeRotationYMatrix(yaw);
	angles.setYaw(angles.getYaw() + yaw);
}

//=============================================================================
// Rotates the camera on the Z axis
//=============================================================================
void Camera::rotateZ(float roll)
{
	//rotation *= Matrix4f::makeRotationZMatrix(roll);
	angles.setRoll(angles.getRoll() + roll);
}