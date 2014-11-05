#pragma once

//=============================================================================
//
//		Template for vectors
//
//=============================================================================


#include "File/File.h"

#include "Math.h"
#include "Platform.h"


namespace myengine {

	template<unsigned SIZE, typename T>
	class Vector
	{
	public:
			Vector();
			Vector(const T vector[SIZE]);

		operator T*();
		operator const T*() const;
		
		bool					operator==(const Vector<SIZE, T> &a) const;
		bool					operator!=(const Vector<SIZE, T> &a) const;
		
		Vector<SIZE, T>			&operator+=(const Vector<SIZE, T> &a);
		Vector<SIZE, T>			&operator-=(const Vector<SIZE, T> &a);
		Vector<SIZE, T>			&operator*=(const T n);
		Vector<SIZE, T>			&operator/=(const T n);

		const Vector<SIZE, T>	operator+(const Vector<SIZE, T> &a) const;
		const Vector<SIZE, T>	operator-() const;
		const Vector<SIZE, T>	operator-(const Vector<SIZE, T> &a) const;
		const Vector<SIZE, T>	operator*(const T n) const;
		const Vector<SIZE, T>	operator/(const T n) const;

		bool					isZero() const;

		const T					*get() const;
		void					set(const T vector[SIZE]);

		void					readFromFile(File *file);
		void					writeToFile(File *file) const;


	private:
		T	vector[SIZE];
	};


	//=========================================================================
	// Constructor
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T>::Vector()
	{
		for(unsigned i = 0; i < SIZE; i++) {
			vector[i] = 0;
		}
	}

	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T>::Vector(const T vector[SIZE])
	{
		for(unsigned i = 0; i < SIZE; i++) {
			this->vector[i] = vector[i];
		}
	}

	//=========================================================================
	// Overload of conversion operator T*
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T>::operator T*()
	{
		return vector;
	}

	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T>::operator const T*() const
	{
		return vector;
	}
	
	//=========================================================================
	// Overload of operator==
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline bool Vector<SIZE, T>::operator==(const Vector<SIZE, T> &a) const
	{
		for(unsigned i = 0; i < SIZE; i++) {
			if(vector[i] != a.vector[i]) {
				return false;
			}
		}
		return true;
	}

	//=========================================================================
	// Overload of operator!=
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline bool Vector<SIZE, T>::operator!=(const Vector<SIZE, T> &a) const
	{
		for(unsigned i = 0; i < SIZE; i++) {
			if(vector[i] != a.vector[i]) {
				return true;
			}
		}
		return false;
	}

	//=========================================================================
	// Overload of operator+=
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T> &Vector<SIZE, T>::operator+=(const Vector<SIZE, T> &a)
	{
		for(unsigned i = 0; i < SIZE; i++) {
			this->vector[i] += a.vector[i];
		}
		return *this;
	}

	//=========================================================================
	// Overload of operator-=
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T> &Vector<SIZE, T>::operator-=(const Vector<SIZE, T> &a)
	{
		for(unsigned i = 0; i < SIZE; i++) {
			this->vector[i] -= a.vector[i];
		}
		return *this;
	}

	//=========================================================================
	// Overload of operator*=
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T> &Vector<SIZE, T>::operator*=(const T n)
	{
		for(unsigned i = 0; i < SIZE; i++) {
			vector[i] *= n;
		}
		return *this;
	}

	//=========================================================================
	// Overload of operator/=
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline Vector<SIZE, T> &Vector<SIZE, T>::operator/=(const T n)
	{
		for(unsigned i = 0; i < SIZE; i++) {
			vector[i] /= n;
		}
		return *this;
	}

	//=========================================================================
	// Overload of operator+
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline const Vector<SIZE, T> Vector<SIZE, T>::operator+(const Vector<SIZE, T> &a) const
	{
		return Vector<SIZE, T>(*this) += a;
	}

	//=========================================================================
	// Overload of operator-
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline const Vector<SIZE, T> Vector<SIZE, T>::operator-() const
	{
		return Vector<SIZE, T>(*this) *= -1;
	}

	template<unsigned SIZE, typename T>
	inline const Vector<SIZE, T> Vector<SIZE, T>::operator-(const Vector<SIZE, T> &a) const
	{
		return Vector<SIZE, T>(*this) -= a;
	}

	//=========================================================================
	// Overload of operator*
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline const Vector<SIZE, T> Vector<SIZE, T>::operator*(const T n) const
	{
		return Vector<SIZE, T>(*this) *= n;
	}

	//=========================================================================
	// Overload of operator/
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline const Vector<SIZE, T> Vector<SIZE, T>::operator/(const T n) const
	{
		return Vector<SIZE, T>(*this) /= n;
	}

	//=========================================================================
	// Returns true if the vector magnitude is zero
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline bool Vector<SIZE, T>::isZero() const
	{
		for(unsigned i = 0; i < SIZE; i++) {
			if(vector[i] != 0) return false;
		}

		return true;
	}
	
	//=========================================================================
	// Returns the vector as array
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline const T *Vector<SIZE, T>::get() const
	{
		return vector;
	}
	
	//=========================================================================
	// Sets the vector
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline void Vector<SIZE, T>::set(const T vector[SIZE])
	{
		for(unsigned i = 0; i < SIZE; i++) {
			this->vector[i] = vector[i];
		}
	}
	
	//=========================================================================
	// Reads the vector from the specified file
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline void Vector<SIZE, T>::readFromFile(File *file)
	{
		file->read(reinterpret_cast<byte*>(vector), sizeof(vector));
	}
	
	//=========================================================================
	// Writes the vector to the specified file
	//=========================================================================
	template<unsigned SIZE, typename T>
	inline void Vector<SIZE, T>::writeToFile(File *file) const
	{
		file->write(reinterpret_cast<const byte*>(vector), sizeof(vector));
	}
	
	
	template<typename T>
	class Vector<2, T>
	{
	public:
			Vector();
			Vector(const T vector[2]);
			Vector(T x, T y);

		operator			T*();
		operator			const T*() const;
			
		bool				operator==(const Vector<2, T> &a) const;
		bool				operator!=(const Vector<2, T> &a) const;

		Vector<2, T>		&operator+=(const Vector<2, T> &a);
		Vector<2, T>		&operator-=(const Vector<2, T> &a);
		Vector<2, T>		&operator*=(const T n);
		Vector<2, T>		&operator/=(const T n);

		const Vector<2, T>	operator+(const Vector<2, T> &a) const;
		const Vector<2, T>	operator-() const;
		const Vector<2, T>	operator-(const Vector<2, T> &a) const;
		const Vector<2, T>	operator*(const T n) const;
		const Vector<2, T>	operator/(const T n) const;

		bool				isNormalized() const;
		bool				isZero() const;

		const T				*get() const;
		void				get(T &x, T &y) const;
		T					getX() const;
		T					getY() const;

		void				set(const T vector[2]);
		void				set(T x, T y);
		void				setX(T x);
		void				setY(T y);

		Vector<2, int>		toVec2i() const;
		Vector<2, float>	toVec2f() const;

		T					distance(const Vector<2, T> &a) const;
		T					magnitude() const;
		void				normalize();
		
		void				readFromFile(File *file);
		void				writeToFile(File *file) const;


	private:
		T	vector[2];
	};
	
	
	typedef Vector<2, int>		Vec2i;
	typedef Vector<2, float>	Vec2f;

	
	//=========================================================================
	// Constructor
	//=========================================================================
	template<typename T>
	inline Vector<2, T>::Vector()
	{
		this->vector[0] = 0;
		this->vector[1] = 0;
	}

	template<typename T>
	inline Vector<2, T>::Vector(const T vector[2])
	{
		this->vector[0] = vector[0];
		this->vector[1] = vector[1];
	}

	template<typename T>
	inline Vector<2, T>::Vector(T x, T y)
	{
		this->vector[0] = x;
		this->vector[1] = y;
	}

	//=========================================================================
	// Overload of conversion operator T*
	//=========================================================================
	template<typename T>
	inline Vector<2, T>::operator T*()
	{
		return vector;
	}

	template<typename T>
	inline Vector<2, T>::operator const T*() const
	{
		return vector;
	}
	
	//=========================================================================
	// Overload of operator==
	//=========================================================================
	template<typename T>
	inline bool Vector<2, T>::operator==(const Vector<2, T> &a) const
	{
		return (vector[0] == a.vector[0]) && (vector[1] == a.vector[1]);
	}

	//=========================================================================
	// Overload of operator!=
	//=========================================================================
	template<typename T>
	inline bool Vector<2, T>::operator!=(const Vector<2, T> &a) const
	{
		return (vector[0] != a.vector[0]) || (vector[1] != a.vector[1]);
	}
		
	//=========================================================================
	// Overload of operator+=
	//=========================================================================
	template<typename T>
	inline Vector<2, T> &Vector<2, T>::operator+=(const Vector<2, T> &a)
	{
		this->vector[0] += a.vector[0];
		this->vector[1] += a.vector[1];
		return *this;
	}

	//=========================================================================
	// Overload of operator-=
	//=========================================================================
	template<typename T>
	inline Vector<2, T> &Vector<2, T>::operator-=(const Vector<2, T> &a)
	{
		this->vector[0] -= a.vector[0];
		this->vector[1] -= a.vector[1];
		return *this;
	}

	//=========================================================================
	// Overload of operator*=
	//=========================================================================
	template<typename T>
	inline Vector<2, T> &Vector<2, T>::operator*=(const T n)
	{
		this->vector[0] *= n;
		this->vector[1] *= n;
		return *this;
	}

	//=========================================================================
	// Overload of operator/=
	//=========================================================================
	template<typename T>
	inline Vector<2, T> &Vector<2, T>::operator/=(const T n)
	{
		this->vector[0] /= n;
		this->vector[1] /= n;
		return *this;
	}

	//=========================================================================
	// Overload of operator+
	//=========================================================================
	template<typename T>
	inline const Vector<2, T> Vector<2, T>::operator+(const Vector<2, T> &a) const
	{
		return Vector<2, T>(*this) += a;
	}

	//=========================================================================
	// Overload of operator-
	//=========================================================================
	template<typename T>
	inline const Vector<2, T> Vector<2, T>::operator-() const
	{
		return Vector<2, T>(-this->vector[0], -this->vector[1]);
	}

	template<typename T>
	inline const Vector<2, T> Vector<2, T>::operator-(const Vector<2, T> &a) const
	{
		return Vector<2, T>(*this) -= a;
	}

	//=========================================================================
	// Overload of operator*
	//=========================================================================
	template<typename T>
	inline const Vector<2, T> Vector<2, T>::operator*(const T n) const
	{
		return Vector<2, T>(*this) *= n;
	}

	//=========================================================================
	// Overload of operator/
	//=========================================================================
	template<typename T>
	inline const Vector<2, T> Vector<2, T>::operator/(const T n) const
	{
		return Vector<2, T>(*this) /= n;
	}
	
	//=========================================================================
	// Returns true if all the components of the vector are zero
	//=========================================================================
	template<typename T>
	inline bool Vector<2, T>::isZero() const
	{
		return (vector[0] == 0) && (vector[1] == 0);
	}

	template<>
	inline bool Vector<2, float>::isZero() const
	{
		return fzero(vector[0]) && fzero(vector[1]);
	}

	
	//=========================================================================
	// Returns the vector as array
	//=========================================================================
	template<typename T>
	inline const T *Vector<2, T>::get() const
	{
		return vector;
	}

	//=========================================================================
	// Returns the components of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::get(T &x, T &y) const
	{
		x = this->vector[0];
		y = this->vector[1];
	}

	//=========================================================================
	// Returns the X component of the vector
	//=========================================================================
	template<typename T>
	inline T Vector<2, T>::getX() const
	{
		return this->vector[0];
	}

	//=========================================================================
	// Returns the Y component of the vector
	//=========================================================================
	template<typename T>
	inline T Vector<2, T>::getY() const
	{
		return this->vector[1];
	}
	
	//=========================================================================
	// Sets the vector
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::set(const T vector[2])
	{
		this->vector[0] = vector[0];
		this->vector[1] = vector[1];
	}

	//=========================================================================
	// Sets the components of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::set(T x, T y)
	{
		this->vector[0] = x;
		this->vector[1] = y;
	}

	//=========================================================================
	// Sets the X component of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::setX(T x)
	{
		this->vector[0] = x;
	}

	//=========================================================================
	// Sets the Y component of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::setY(T y)
	{
		this->vector[1] = y;
	}
		
	//=========================================================================
	// Returns an integer Vector
	//=========================================================================
	template<>
	inline Vector<2, int> Vector<2, float>::toVec2i() const
	{
		return Vector<2, int>(
			static_cast<int>(this->vector[0]),
			static_cast<int>(this->vector[1])
		);
	}
	
	//=========================================================================
	// Returns a float Vector
	//=========================================================================
	template<>
	inline Vector<2, float> Vector<2, int>::toVec2f() const
	{
		return Vector<2, float>(
			static_cast<float>(this->vector[0]),
			static_cast<float>(this->vector[1])
		);
	}

	//=============================================================================
	// Returns the distance between the specified points
	//=============================================================================
	template<typename T>
	inline T Vector<2, T>::distance(const Vector<2, T> &a) const
	{
		return sqrt(
			square<T>(this->vector[0] + a.vector[0]) +
			square<T>(this->vector[1] + a.vector[1])
		);
	}

	//=============================================================================
	// Returns the magnitude of the vector
	//=============================================================================
	template<typename T>
	inline T Vector<2, T>::magnitude() const
	{
		return sqrt(square<T>(this->vector[0]) + square<T>(this->vector[1]));
	}

	//=========================================================================
	// Normalizes the vector
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::normalize()
	{
		*this /= magnitude();
	}
	
	//=========================================================================
	// Reads the vector from the specified file
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::readFromFile(File *file)
	{
		file->read(reinterpret_cast<byte*>(vector), sizeof(vector));
	}
	
	//=========================================================================
	// Writes the vector to the specified file
	//=========================================================================
	template<typename T>
	inline void Vector<2, T>::writeToFile(File *file) const
	{
		file->write(reinterpret_cast<const byte*>(vector), sizeof(vector));
	}
	
	
	template<typename T>
	class Vector<3, T>
	{
	public:
			Vector();
			Vector(const T vector[3]);
			Vector(T x, T y, T z);

		operator			T*();
		operator			const T*() const;
		
		bool				operator==(const Vector<3, T> &a) const;
		bool				operator!=(const Vector<3, T> &a) const;

		Vector<3, T>		&operator+=(const Vector<3, T> &a);
		Vector<3, T>		&operator-=(const Vector<3, T> &a);
		Vector<3, T>		&operator*=(const T n);
		Vector<3, T>		&operator/=(const T n);

		const Vector<3, T>	operator+(const Vector<3, T> &a) const;
		const Vector<3, T>	operator-() const;
		const Vector<3, T>	operator-(const Vector<3, T> &a) const;
		const Vector<3, T>	operator*(const T n) const;
		const Vector<3, T>	operator/(const T n) const;
		
		bool				isZero() const;

		const T				*get() const;
		void				get(T &x, T &y, T &z) const;
		T					getX() const;
		T					getY() const;
		T					getZ() const;

		void				set(const T vector[3]);
		void				set(T x, T y, T z);
		void				setX(T x);
		void				setY(T y);
		void				setZ(T z);

		const Vector<3, int>	toVec3i() const;
		const Vector<3, float>	toVec3f() const;

		const Vector<3, T>	cross(const Vector<3, T> &a) const;
		T					dot(const Vector<3, T> &a) const;
		T					distance(const Vector<3, T> &a) const;
		T					magnitude() const;
		void				normalize();
		
		void				readFromFile(File *file);
		void				writeToFile(File *file) const;

		
	private:
		T	vector[3];
	};

	
	typedef Vector<3, int>		Vec3i;
	typedef Vector<3, float>	Vec3f;

	
	//=========================================================================
	// Constructor
	//=========================================================================
	template<typename T>
	inline Vector<3, T>::Vector()
	{
		this->vector[0] = 0;
		this->vector[1] = 0;
		this->vector[2] = 0;
	}

	template<typename T>
	inline Vector<3, T>::Vector(const T vector[3])
	{
		this->vector[0] = vector[0];
		this->vector[1] = vector[1];
		this->vector[2] = vector[2];
	}

	template<typename T>
	inline Vector<3, T>::Vector(T x, T y, T z)
	{
		this->vector[0] = x;
		this->vector[1] = y;
		this->vector[2] = z;
	}

	
	//=========================================================================
	// Overload of conversion operator T*
	//=========================================================================
	template<typename T>
	inline Vector<3, T>::operator T*()
	{
		return vector;
	}

	template<typename T>
	inline Vector<3, T>::operator const T*() const
	{
		return vector;
	}
	
	//=========================================================================
	// Overload of operator==
	//=========================================================================
	template<typename T>
	inline bool Vector<3, T>::operator==(const Vector<3, T> &a) const
	{
		return (
			(vector[0] == a.vector[0]) &&
			(vector[1] == a.vector[1]) &&
			(vector[2] == a.vector[2])
		);
	}

	//=========================================================================
	// Overload of operator!=
	//=========================================================================
	template<typename T>
	inline bool Vector<3, T>::operator!=(const Vector<3, T> &a) const
	{
		return (
			(vector[0] != a.vector[0]) ||
			(vector[1] != a.vector[1]) ||
			(vector[2] != a.vector[2])
		);
	}
		
	//=========================================================================
	// Overload of operator+=
	//=========================================================================
	template<typename T>
	inline Vector<3, T> &Vector<3, T>::operator+=(const Vector<3, T> &a)
	{
		this->vector[0] += a.vector[0];
		this->vector[1] += a.vector[1];
		this->vector[2] += a.vector[2];
		return *this;
	}

	//=========================================================================
	// Overload of operator-=
	//=========================================================================
	template<typename T>
	inline Vector<3, T> &Vector<3, T>::operator-=(const Vector<3, T> &a)
	{
		this->vector[0] -= a.vector[0];
		this->vector[1] -= a.vector[1];
		this->vector[2] -= a.vector[2];
		return *this;
	}

	//=========================================================================
	// Overload of operator*=
	//=========================================================================
	template<typename T>
	inline Vector<3, T> &Vector<3, T>::operator*=(const T n)
	{
		this->vector[0] *= n;
		this->vector[1] *= n;
		this->vector[2] *= n;
		return *this;
	}

	//=========================================================================
	// Overload of operator/=
	//=========================================================================
	template<typename T>
	inline Vector<3, T> &Vector<3, T>::operator/=(const T n)
	{
		this->vector[0] /= n;
		this->vector[1] /= n;
		this->vector[2] /= n;
		return *this;
	}

	//=========================================================================
	// Overload of operator+
	//=========================================================================
	template<typename T>
	inline const Vector<3, T> Vector<3, T>::operator+(const Vector<3, T> &a) const
	{
		return Vector<3, T>(*this) += a;
	}

	//=========================================================================
	// Overload of operator-
	//=========================================================================
	template<typename T>
	inline const Vector<3, T> Vector<3, T>::operator-() const
	{
		return Vector<3, T>(-this->vector[0], -this->vector[1], -this->vector[2]);
	}

	template<typename T>
	inline const Vector<3, T> Vector<3, T>::operator-(const Vector<3, T> &a) const
	{
		return Vector<3, T>(*this) -= a;
	}

	//=========================================================================
	// Overload of operator*
	//=========================================================================
	template<typename T>
	inline const Vector<3, T> Vector<3, T>::operator*(const T n) const
	{
		return Vector<3, T>(*this) *= n;
	}

	//=========================================================================
	// Overload of operator/
	//=========================================================================
	template<typename T>
	inline const Vector<3, T> Vector<3, T>::operator/(const T n) const
	{
		return Vector<3, T>(*this) /= n;
	}
	
	//=========================================================================
	// Returns true if all the components of the vector are zero
	//=========================================================================
	template<typename T>
	inline bool Vector<3, T>::isZero() const
	{
		return (vector[0] == 0) && (vector[1] == 0) && (vector[2] == 0);
	}

	template<>
	inline bool Vector<3, float>::isZero() const
	{
		return fzero(vector[0]) && fzero(vector[1]) && fzero(vector[2]);
	}
	
	//=========================================================================
	// Returns the vector as array
	//=========================================================================
	template<typename T>
	inline const T *Vector<3, T>::get() const
	{
		return vector;
	}
	
	//=========================================================================
	// Returns the components of the vecor
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::get(T &x, T &y, T &z) const
	{
		x = this->vector[0];
		y = this->vector[1];
		z = this->vector[2];
	}

	//=========================================================================
	// Returns the X component of the vecor
	//=========================================================================
	template<typename T>
	inline T Vector<3, T>::getX() const
	{
		return this->vector[0];
	}

	//=========================================================================
	// Returns the Y component of the vecor
	//=========================================================================
	template<typename T>
	inline T Vector<3, T>::getY() const
	{
		return this->vector[1];
	}

	//=========================================================================
	// Returns the Z component of the vecor
	//=========================================================================
	template<typename T>
	inline T Vector<3, T>::getZ() const
	{
		return this->vector[2];
	}

	//=========================================================================
	// Sets the vector
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::set(const T vector[3])
	{
		this->vector[0] = vector[0];
		this->vector[1] = vector[1];
		this->vector[2] = vector[2];
	}

	//=========================================================================
	// Sets the components of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::set(T x, T y, T z)
	{
		this->vector[0] = x;
		this->vector[1] = y;
		this->vector[2] = z;
	}

	//=========================================================================
	// Sets the x component of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::setX(T x)
	{
		this->vector[0] = x;
	}

	//=========================================================================
	// Sets the y component of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::setY(T y)
	{
		this->vector[1] = y;
	}

	//=========================================================================
	// Sets the z component of the vector
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::setZ(T z)
	{
		this->vector[2] = z;
	}
	
	//=========================================================================
	// Returns an integer Vector
	//=========================================================================
	template<>
	inline const Vector<3, int> Vector<3, float>::toVec3i() const
	{
		return Vector<3, int>(
			static_cast<int>(vector[0]),
			static_cast<int>(vector[1]),
			static_cast<int>(vector[2])
		);
	}
	
	//=========================================================================
	// Returns a float Vector
	//=========================================================================
	template<>
	inline const Vector<3, float> Vector<3, int>::toVec3f() const
	{
		return Vector<3, float>(
			static_cast<float>(vector[0]),
			static_cast<float>(vector[1]),
			static_cast<float>(vector[2])
		);
	}

	//=========================================================================
	// Returns the cross product with the specified vector
	//=========================================================================
	template<typename T>
	const Vector<3, T> Vector<3, T>::cross(const Vector<3, T> &a) const
	{
		return Vector<3, T>(
			this->vector[1] * a.vector[2] - this->vector[2] * a.vector[1],
			this->vector[2] * a.vector[0] - this->vector[0] * a.vector[2],
			this->vector[0] * a.vector[1] - this->vector[1] * a.vector[0]
		);
	}

	//=========================================================================
	// Returns the dot product with the specified vector
	//=========================================================================
	template<typename T>
	T Vector<3, T>::dot(const Vector<3, T> &a) const
	{
		return (
			this->vector[0] * a.vector[0] +
			this->vector[1] * a.vector[1] +
			this->vector[2] * a.vector[2]
		);
	}
	
	//=========================================================================
	// Returns the distance from the specified vector
	//=========================================================================
	template<typename T>
	T Vector<3, T>::distance(const Vector<3, T> &a) const
	{
		return sqrt(
			square<T>(this->vector[0] + a.vector[0]) +
			square<T>(this->vector[1] + a.vector[1]) +
			square<T>(this->vector[2] + a.vector[2])
		);
	}

	//=========================================================================
	// Returns the magnitude of the vector
	//=========================================================================
	template<typename T>
	T Vector<3, T>::magnitude() const
	{
		return sqrt(
			square<T>(this->vector[0]) +
			square<T>(this->vector[1]) +
			square<T>(this->vector[2])
		);
	}

	//=========================================================================
	// Normalizes the vector
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::normalize()
	{
		*this /= magnitude();
	}

	//=========================================================================
	// Reads the vector from the specified file
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::readFromFile(File *file)
	{
		file->read(reinterpret_cast<byte*>(vector), sizeof(vector));
	}
	
	//=========================================================================
	// Writes the vector to the specified file
	//=========================================================================
	template<typename T>
	inline void Vector<3, T>::writeToFile(File *file) const
	{
		file->write(reinterpret_cast<const byte*>(vector), sizeof(vector));
	}

}