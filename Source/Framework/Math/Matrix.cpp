#include "Matrix.h"
//#include "Memory.h"

//#include "Math/Math.h"

namespace myengine {
	
//=============================================================================
// Overload of operator==
//=============================================================================
template<>
bool Matrix<4, float>::operator==(const Matrix<4, float> &a) const
{
	for(unsigned i = 0; i < 16; i++) {
		if(!fequal(matrix[i], a.matrix[i])) {
			return false;
		}
	}
	return true;
}

//=============================================================================
// Overload of operator!=
//=============================================================================
template<>
bool Matrix<4, float>::operator!=(const Matrix<4, float> &a) const
{
	for(unsigned i = 0; i < 16; i++) {
		if(!fequal(matrix[i], a.matrix[i])) {
			return true;
		}
	}
	return false;
}

}
/*
//=============================================================================
// Destructor
//=============================================================================
//Matrix4::~Matrix4() {}

//=============================================================================
// Makes the identity matrix
//=============================================================================
void Matrix4::identity()
{
	zero();
	matrix[M_11] = 1;
	matrix[M_22] = 1;
	matrix[M_33] = 1;
	matrix[M_44] = 1;
}

//=============================================================================
// Sets all elements to zero
//=============================================================================
void Matrix4::zero()
{
	Memory::memset(matrix, 0, sizeof(matrix));
}

//=============================================================================
// Makes a perspective projection matrix (gluPerspective)
//=============================================================================
void Matrix4::makePerspective(float fovy, float aspect, float near, float far)
{
	zero();
	float f = 1.0f / tan(fovy / 2 * DEG2RAD);
	matrix[M_11] = f / aspect;
	matrix[M_22] = f;
	matrix[M_33] = (near + far) / (near - far);
	matrix[M_34] = (2 * near * far) / (near - far);
	matrix[M_43] = -1;
}

//=============================================================================
// Makes a rotation matrix for the Y axis
//=============================================================================
void Matrix4::makeTranslate(float x, float y, float z)
{
	identity();
	matrix[M_14] = x;
	matrix[M_24] = y;
	matrix[M_34] = z;
}

//=============================================================================
// Extracts the 6 planes of the viewing frustum
//=============================================================================
void Matrix4::extractViewingFrustumPlanes(Plane *planes)
{
	planes[0].set(
		matrix[M_41] + matrix[M_11],
		matrix[M_42] + matrix[M_12],
		matrix[M_43] + matrix[M_13],
		matrix[M_44] + matrix[M_14]
	);
	planes[1].set(
		matrix[M_41] - matrix[M_11],
		matrix[M_42] - matrix[M_12],
		matrix[M_43] - matrix[M_13],
		matrix[M_44] - matrix[M_14]
	);
	planes[2].set(
		matrix[M_41] + matrix[M_21],
		matrix[M_42] + matrix[M_22],
		matrix[M_43] + matrix[M_23],
		matrix[M_44] + matrix[M_24]
	);

	planes[3].set(
		matrix[M_41] - matrix[M_21],
		matrix[M_42] - matrix[M_22],
		matrix[M_43] - matrix[M_23],
		matrix[M_44] - matrix[M_24]
	);
	planes[4].set(
		matrix[M_41] + matrix[M_21],
		matrix[M_42] + matrix[M_22],
		matrix[M_43] + matrix[M_23],
		matrix[M_44] + matrix[M_24]
	);
	planes[5].set(
		matrix[M_41] - matrix[M_31],
		matrix[M_42] - matrix[M_32],
		matrix[M_43] - matrix[M_33],
		matrix[M_44] - matrix[M_34]
	);
}*/