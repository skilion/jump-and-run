#pragma once
#ifndef __MATRIX_H__
#define __MATRIX_H__


//=============================================================================
//
//		Template for matrices (stored as row-major)
//
//=============================================================================


//#include "Plane.h"
#include "Math.h"
#include "Memory.h"

#include "File/File.h"


namespace myengine {

	template<unsigned N, typename T>
	class BaseMatrix
	{
	public:
			BaseMatrix();
			BaseMatrix(const float matrix[N * N]);

		operator	T*();
		operator	const T*() const;

		const T		*get() const;
		void		set(const T matrix[N * N]);

		void		transpose();

		void		readFromFile(File *file);
		void		writeToFile(File *file) const;


	protected:
		T	matrix[N * N];
	};


	//=============================================================================
	// Constructor
	//=============================================================================
	template<unsigned N, typename T>
	inline BaseMatrix<N, T>::BaseMatrix()
	{
		unsigned i = 0;
		for(unsigned j = 0; j < N; j++) {
			for(unsigned k = 0; k < N; k++) {
				if(j == k)	matrix[i] = 1;
				else		matrix[i] = 0;
				i++;
			}
		}
	}

	template<unsigned N, typename T>
	inline BaseMatrix<N, T>::BaseMatrix(const float matrix[N * N])
	{
		Memory::memcpy(this->matrix, matrix, sizeof(T) * 16);
	}
		
	//=============================================================================
	// Overload of conversion operator T*
	//=============================================================================
	template<unsigned N, typename T>
	inline BaseMatrix<N, T>::operator T*()
	{
		return matrix;
	}

	template<unsigned N, typename T>
	inline BaseMatrix<N, T>::operator const T*() const
	{
		return matrix;
	}
	
	//=========================================================================
	// Returns the matrix as array
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline const T *BaseMatrix<SIZE, T>::get() const
	{
		return matrix;
	}
	
	//=========================================================================
	// Sets the matrix
	//=========================================================================
	template<unsigned N, typename T>
	inline void BaseMatrix<N, T>::set(const T matrix[N * N])
	{
		Memory::memcpy(this->matrix, matrix, sizeof(T) * 16);
	}

	//=========================================================================
	// Transpose the matrix
	//=========================================================================
	template<unsigned N, typename T>
	inline void BaseMatrix<N, T>::transpose()
	{
		for(unsigned i = 0; i < N - 1; i++) { //row
			for(unsigned j = i + 1; j < N; j++) { //column
				unsigned swap = matrix[i * N + j];
				matrix[i * N + j] = matrix[j * N + i];
				matrix[j * N + i] = swap;
			}
		}
	}


	template<unsigned N, typename T>
	class Matrix: public BaseMatrix<N, T>
	{
	public:
			Matrix();
			Matrix(const float matrix[N * N]);

		/*Matrix<4, T>		&operator+=(const Matrix<N, T> &a);
		Matrix<N, T>		&operator-=(const Matrix<N, T> &a);
		Matrix<N, T>		&operator*=(const T n);
		Matrix<N, T>		&operator*=(const Matrix<N, T> &a);
		Matrix<N, T>		&operator/=(const T n);

		const Matrix<N, T>	operator-() const;
		const Matrix<N, T>	operator+(const Matrix<N, T> &a) const;
		const Matrix<N, T>	operator-(const Matrix<N, T> &a) const;
		const Matrix<N, T>	operator*(const T n) const;
		const Matrix<N, T>	operator*(const Matrix<N, T> &a) const;
		const Matrix<N, T>	operator/(const T n) const;*/

		//void			identity();
		//void			zero();
	};


	///////////////////////////////////


	template<typename T>
	class Matrix<4, T>: public BaseMatrix<4, T>
	{
	public:
		enum MatrixElements {
			M_11 = 0,
			M_12 = 1,
			M_13 = 2,
			M_14 = 3,
			M_21 = 4,
			M_22 = 5,
			M_23 = 6,
			M_24 = 7,
			M_31 = 8,
			M_32 = 9,
			M_33 = 10,
			M_34 = 11,
			M_41 = 12,
			M_42 = 13,
			M_43 = 14,
			M_44 = 15
		};

			Matrix();
			Matrix(const T matrix[16]);

		Matrix<4, T>		&operator+=(const Matrix<4, T> &a);
		Matrix<4, T>		&operator-=(const Matrix<4, T> &a);
		Matrix<4, T>		&operator*=(const T n);
		Matrix<4, T>		&operator*=(const Matrix<4, T> &a);
		Matrix<4, T>		&operator/=(const T n);

		const Matrix<4, T>	operator-() const;
		const Matrix<4, T>	operator+(const Matrix<4, T> &a) const;
		const Matrix<4, T>	operator-(const Matrix<4, T> &a) const;
		const Matrix<4, T>	operator*(const T n) const;
		const Matrix<4, T>	operator*(const Matrix<4, T> &a) const;
		//const Matrix<4, T>	operator/(const T n) const;

		bool				operator==(const Matrix<4, T> &a) const;
		bool				operator!=(const Matrix<4, T> &a) const;

		//void			identity();
		//void			zero();

		/*void			makePerspective(T fovy, T aspect, T near, T far);
		void			makeRotationX(T pitch);
		void			makeRotationY(T yaw);
		void			makeRotationZ(T roll);
		void			makeTranslate(T x, T y, T z);

		void			extractViewingFrustumPlanes(Plane *planes);*/

		static const Matrix<4, T>	makeRotationXMatrix(float pitch);
		static const Matrix<4, T>	makeRotationYMatrix(float yaw);
		static const Matrix<4, T>	makeRotationZMatrix(float roll);
	};


	typedef Matrix<4, float> Matrix4f;

	//=============================================================================
	// Constructor
	//=============================================================================
	template<typename T>
	inline Matrix<4, T>::Matrix(): BaseMatrix<4, T>::BaseMatrix()
	{
	}

	template<typename T>
	inline Matrix<4, T>::Matrix(const T matrix[16]): BaseMatrix<4, T>::BaseMatrix(matrix)
	{
	}
	
	//=============================================================================
	// Overload of operator+=
	//=============================================================================
	template<typename T>
	Matrix<4, T> &Matrix<4, T>::operator+=(const Matrix<4, T> &a)
	{
		this->matrix[0]	+= a.matrix[0];
		this->matrix[1]	+= a.matrix[1];
		this->matrix[2]	+= a.matrix[2];
		this->matrix[3]	+= a.matrix[3];
		this->matrix[4]	+= a.matrix[4];
		this->matrix[5]	+= a.matrix[5];
		this->matrix[6]	+= a.matrix[6];
		this->matrix[7]	+= a.matrix[7];
		this->matrix[8]	+= a.matrix[8];
		this->matrix[9]	+= a.matrix[9];
		this->matrix[10]	+= a.matrix[10];
		this->matrix[11]	+= a.matrix[11];
		this->matrix[12]	+= a.matrix[12];
		this->matrix[13]	+= a.matrix[13];
		this->matrix[14]	+= a.matrix[14];
		this->matrix[15]	+= a.matrix[15];
		return *this;
	}
	
	//=============================================================================
	// Overload of operator-=
	//=============================================================================
	template<typename T>
	Matrix<4, T> &Matrix<4, T>::operator-=(const Matrix<4, T> &a)
	{
		this->matrix[0]	-= a.matrix[0];
		this->matrix[1]	-= a.matrix[1];
		this->matrix[2]	-= a.matrix[2];
		this->matrix[3]	-= a.matrix[3];
		this->matrix[4]	-= a.matrix[4];
		this->matrix[5]	-= a.matrix[5];
		this->matrix[6]	-= a.matrix[6];
		this->matrix[7]	-= a.matrix[7];
		this->matrix[8]	-= a.matrix[8];
		this->matrix[9]	-= a.matrix[9];
		this->matrix[10]	-= a.matrix[10];
		this->matrix[11]	-= a.matrix[11];
		this->matrix[12]	-= a.matrix[12];
		this->matrix[13]	-= a.matrix[13];
		this->matrix[14]	-= a.matrix[14];
		this->matrix[15]	-= a.matrix[15];
		return *this;
	}
	
	//=============================================================================
	// Overload of operator*=
	//=============================================================================
	template<typename T>
	Matrix<4, T> &Matrix<4, T>::operator*=(const T n)
	{
		this->matrix[0]	*= n;
		this->matrix[1]	*= n;
		this->matrix[2]	*= n;
		this->matrix[3]	*= n;
		this->matrix[4]	*= n;
		this->matrix[5]	*= n;
		this->matrix[6]	*= n;
		this->matrix[7]	*= n;
		this->matrix[8]	*= n;
		this->matrix[9]	*= n;
		this->matrix[10]	*= n;
		this->matrix[11]	*= n;
		this->matrix[12]	*= n;
		this->matrix[13]	*= n;
		this->matrix[14]	*= n;
		this->matrix[15]	*= n;
		return *this;
	}
	
	//=============================================================================
	// Overload of operator*=
	//=============================================================================
	template<typename T>
	Matrix<4, T> &Matrix<4, T>::operator*=(const Matrix<4, T> &a)
	{
		if(this == &a) return *this = *this * a;

		float column[4];
		for(unsigned i = 0; i < 4; i++) { //Columns
			column[0] = this->matrix[0 * 4 + i];
			column[1] = this->matrix[1 * 4 + i];
			column[2] = this->matrix[2 * 4 + i];
			column[3] = this->matrix[3 * 4 + i];
			
			this->matrix[0 * 4 + i] = (
				column[0] * a.matrix[0 * 4 + 0] +
				column[1] * a.matrix[0 * 4 + 1] +
				column[2] * a.matrix[0 * 4 + 2] +
				column[3] * a.matrix[0 * 4 + 3]
			);
			this->matrix[1 * 4 + i] = (
				column[0] * a.matrix[1 * 4 + 0] +
				column[1] * a.matrix[1 * 4 + 1] +
				column[2] * a.matrix[1 * 4 + 2] +
				column[3] * a.matrix[1 * 4 + 3]
			);
			this->matrix[2 * 4 + i] = (
				column[0] * a.matrix[2 * 4 + 0] +
				column[1] * a.matrix[2 * 4 + 1] +
				column[2] * a.matrix[2 * 4 + 2] +
				column[3] * a.matrix[2 * 4 + 3]
			);
			this->matrix[3 * 4 + i] = (
				column[0] * a.matrix[3 * 4 + 0] +
				column[1] * a.matrix[3 * 4 + 1] +
				column[2] * a.matrix[3 * 4 + 2] +
				column[3] * a.matrix[3 * 4 + 3]
			);
		}

		return *this;
	}
	
	//=============================================================================
	// Overload of operator/=
	//=============================================================================
	template<typename T>
	Matrix<4, T> &Matrix<4, T>::operator/=(const T n)
	{
		this->matrix[0]	/= n;
		this->matrix[1]	/= n;
		this->matrix[2]	/= n;
		this->matrix[3]	/= n;
		this->matrix[4]	/= n;
		this->matrix[5]	/= n;
		this->matrix[6]	/= n;
		this->matrix[7]	/= n;
		this->matrix[8]	/= n;
		this->matrix[9]	/= n;
		this->matrix[10]	/= n;
		this->matrix[11]	/= n;
		this->matrix[12]	/= n;
		this->matrix[13]	/= n;
		this->matrix[14]	/= n;
		this->matrix[15]	/= n;
		return *this;
	}
	
	//=============================================================================
	// Overload of operator-
	//=============================================================================
	template<typename T>
	inline const Matrix<4, T> Matrix<4, T>::operator-() const
	{
		return Matrix<4, T>(*this) *= -1;
	}
	
	//=============================================================================
	// Overload of operator+
	//=============================================================================
	template<typename T>
	inline const Matrix<4, T> Matrix<4, T>::operator+(const Matrix<4, T> &a) const
	{
		return Matrix<4, T>(*this) += a;
	}
	
	//=============================================================================
	// Overload of operator-
	//=============================================================================
	template<typename T>
	inline const Matrix<4, T> Matrix<4, T>::operator-(const Matrix<4, T> &a) const
	{
		return Matrix<4, T>(*this) -= a;
	}
	
	//=============================================================================
	// Overload of operator*
	//=============================================================================
	template<typename T>
	const Matrix<4, T> Matrix<4, T>::operator*(const Matrix<4, T> &a) const
	{
		float result[16];

		for(unsigned i = 0; i < 4; i++) { //Columns
			unsigned rowIndex = i * 4;
			result[0 * 4 + i] = (
				this->matrix[0 * 4 + i] * a.matrix[0 * 4 + 0] +
				this->matrix[1 * 4 + i] * a.matrix[0 * 4 + 1] +
				this->matrix[2 * 4 + i] * a.matrix[0 * 4 + 2] +
				this->matrix[3 * 4 + i] * a.matrix[0 * 4 + 3]
			);
			result[1 * 4 + i] = (
				this->matrix[0 * 4 + i] * a.matrix[1 * 4 + 0] +
				this->matrix[1 * 4 + i] * a.matrix[1 * 4 + 1] +
				this->matrix[2 * 4 + i] * a.matrix[1 * 4 + 2] +
				this->matrix[3 * 4 + i] * a.matrix[1 * 4 + 3]
			);
			result[2 * 4 + i] = (
				this->matrix[0 * 4 + i] * a.matrix[2 * 4 + 0] +
				this->matrix[1 * 4 + i] * a.matrix[2 * 4 + 1] +
				this->matrix[2 * 4 + i] * a.matrix[2 * 4 + 2] +
				this->matrix[3 * 4 + i] * a.matrix[2 * 4 + 3]
			);
			result[3 * 4 + i] = (
				this->matrix[0 * 4 + i] * a.matrix[3 * 4 + 0] +
				this->matrix[1 * 4 + i] * a.matrix[3 * 4 + 1] +
				this->matrix[2 * 4 + i] * a.matrix[3 * 4 + 2] +
				this->matrix[3 * 4 + i] * a.matrix[3 * 4 + 3]
			);
		}

		return Matrix<4, T>(result);
	}
		
	//=============================================================================
	// Overload of operator==
	//=============================================================================
	template<typename T>
	bool Matrix<4, T>::operator==(const Matrix<4, T> &a) const
	{
		for(unsigned i = 0; i < 16; i++) {
			if(this->matrix[i] != a.matrix[i]) {
				return false;
			}
		}
		return true;
	}
	
	//=============================================================================
	// Overload of operator!=
	//=============================================================================
	template<typename T>
	bool Matrix<4, T>::operator!=(const Matrix<4, T> &a) const
	{
		for(unsigned i = 0; i < 16; i++) {
			if(this->matrix[i] != a.matrix[i]) {
				return true;
			}
		}
		return false;
	}
	
	//=============================================================================
	// Makes a rotation matrix for the X axis
	//=============================================================================
	template<typename T>
	const Matrix<4, T> Matrix<4, T>::makeRotationXMatrix(float pitch)
	{
		Matrix<4, T> matrix;
		float c, s;
		sincos(pitch, s, c);

		matrix[M_11] = 1;
		matrix[M_22] = c;
		matrix[M_23] = -s;
		matrix[M_32] = s;
		matrix[M_33] = c;
		matrix[M_44] = 1;

		return matrix;
	}

	//=============================================================================
	// Makes a rotation matrix for the Y axis
	//=============================================================================
	template<typename T>
	const Matrix<4, T> Matrix<4, T>::makeRotationYMatrix(float yaw)
	{
		Matrix<4, T> matrix;
		float c, s;
		sincos(yaw, s, c);

		matrix[M_11] = c;
		matrix[M_13] = s;
		matrix[M_22] = 1;
		matrix[M_31] = -s;
		matrix[M_33] = c;
		matrix[M_44] = 1;

		return matrix;
	}

	//=============================================================================
	// Makes a rotation matrix for the Z axis
	//=============================================================================
	template<typename T>
	const Matrix<4, T> Matrix<4, T>::makeRotationZMatrix(float roll)
	{
		Matrix<4, T> matrix;
		float c, s;
		sincos(roll, s, c);

		matrix[M_11] = c;
		matrix[M_12] = -s;
		matrix[M_21] = s;
		matrix[M_22] = c;
		matrix[M_33] = 1;
		matrix[M_44] = 1;

		return matrix;
	}
}


#endif //!__MATRIX_H__
