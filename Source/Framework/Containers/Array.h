#pragma once

//=============================================================================
//
//		Template for dynamic arrays
//
//=============================================================================

#include "Math/Math.h"

#include "Platform.h"


namespace myengine {
	
	template<typename T>
	class Array
	{
	public:
			Array();
			Array(unsigned size);
			Array(const Array<T> &array);
			~Array();

		operator T*();
		operator const T*() const;

		Array<T>	&operator=(const Array<T> &array);
		
		/*bool		operator==(const Array<T> &a) const;
		bool		operator!=(const Array<T> &a) const;*/

		//bool		binarySearch(const T &element, unsigned &index, unsigned start, unsigned end);

		void		copyTo(Array<T> &a, unsigned start, unsigned end);
		
		T			*get();

		//unsigned	getLenght() const;
		unsigned	getSize() const;

		void		ensureSize(unsigned size, bool keep = true);

		void		free();
		
		//bool		isEqual(Array<T> &a, unsigned count) const;

		//void		quickSort(unsigned start, unsigned end);

		void		swap(unsigned i1, unsigned i2);


	protected:
		T			*data;


	private:
		void		allocate(unsigned size);
		void		reallocate(unsigned size);

		unsigned	size;
	};


	//=========================================================================
	// Constructor
	//=========================================================================
	template<typename T>
	inline Array<T>::Array()
	{
		data = nullptr;
		size = 0;
	}

	template<typename T>
	inline Array<T>::Array(unsigned size)
	{
		data = nullptr;
		this->size = 0;

		allocate(size);
	}

	template<typename T>
	inline Array<T>::Array(const Array<T> &array)
	{
		data = 0;
		size = 0;

		*this = array;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	template <typename T>
	inline Array<T>::~Array()
	{
		delete [] data;
	}

	//=========================================================================
	// Overload of coversion operator T*
	//=========================================================================
	template <typename T>
	inline Array<T>::operator T*()
	{
		return data;
	}

	template <typename T>
	inline Array<T>::operator const T*() const
	{
		return data;
	}

	//=========================================================================
	// Overload of operator=
	//=========================================================================
	template <typename T>
	Array<T> &Array<T>::operator=(const Array<T> &array)
	{
		if(this != &array) {
			ensureSize(array.size, false);
			for(unsigned i = 0; i < array.size; i++) data[i] = array.data[i];
		}

		return *this;
	}
	
	//=========================================================================
	// Overload of operator==
	//=========================================================================
	/*template <typename T>
	inline bool Array<T>::operator==(const Array<T> &a) const
	{
		return isEqual(a);
	}

	//=========================================================================
	// Overload of operator!=
	//=========================================================================
	template <typename T>
	bool Array<T>::operator!=(const Array<T> &a) const
	{
		return !isEqual(a);
	}*/

	//=========================================================================
	// Performs a binary search
	//=========================================================================
	/*template <typename T>
	bool Array<T>::binarySearch(const T &element, unsigned &index, unsigned start, unsigned end) {
		while(start <= end) {
			index = (start + end) / 2;
			if(data[index] < element) start = index + 1;
			else if(data[index] > element) end = index - 1;
			else return true;
		}

		return false;
	}*/

	//=========================================================================
	// Copies from start to end in the specified array
	//=========================================================================
	template <typename T>
	void Array<T>::copyTo(Array<T> &a, unsigned start, unsigned end)
	{
		ASSERT(start <= end);
		ASSERT(end < size);
		
		unsigned sizeToCopy = end - start;
		a.ensureSize(sizeToCopy);

		for(unsigned i = 0; i < sizeToCopy; i++) a.data[i] = data[i + start];
	}
	
	//=========================================================================
	// Returns a pointer to the array
	//=========================================================================
	template <typename T>
	inline T *Array<T>::get()
	{
		return data;
	}

	//=========================================================================
	// Return the current size
	//=========================================================================
	template <typename T>
	inline unsigned Array<T>::getSize() const
	{
		return size;
	}

	//=========================================================================
	// Ensures there is enough memory
	//=========================================================================
	template <typename T>
	void Array<T>::ensureSize(unsigned size, bool keep)
	{
		if(this->size < size) {
			unsigned increment = this->size << 1;
			if(increment > size) size = increment;
			keep ? reallocate(size) : allocate(size);
		}
	}

	//=========================================================================
	// Frees the allocated memory
	//=========================================================================
	template <typename T>
	inline void Array<T>::free()
	{
		delete [] data;
		data = 0;
		size = 0;
	}

	//=========================================================================
	// Sort the array with the quick sort algorithm
	//=========================================================================
	/*template <typename T>
	void Array<T>::quickSort(unsigned start, unsigned end) //todo: test !
	{
		if(end > start) {
			unsigned pivot = (start + end) >> / 2;
			T pivotValue = data[pivot];

			swap(pivot, end - 1);
			unsigned storeIndex = start;
			for(unsigned i = start; i < end - 1; i++) {
				if(data[i] < pivotValue) {
					swap(i, storeIndex);
					storeIndex = storeIndex + 1;
				}
				swap(storeIndex, end);
			}

			quickSort(data, start, storeIndex - 1);
			quicksort(data, storeIndex + 1, end);
		}
	}*/
	
	//=========================================================================
	// Returns true if the specified array is equal
	//=========================================================================
	/*template <typename T>
	bool Array<T>::isEqual(Array<T> &a, unsigned count) const
	{
		if(size < count) count = size;
		if(a.size < count) count = a.size;
		if(count == 0) return false;

		for(unsigned i = 0; i < count; i++) {
			if(data[i] != a.data[i]) return false;
		}

		return true;
	}*/
	
	//=========================================================================
	// Swaps two elements
	//=========================================================================
	template <typename T>
	void Array<T>::swap(unsigned i1, unsigned i2)
	{
		ASSERT(i1 < size);
		ASSERT(i2 < size);

		T swap = data[i1];
		data[i1] = data[i2];
		data[i2] = swap;
	}

	//=========================================================================
	// Allocates new memory
	//=========================================================================
	template <typename T>
	void Array<T>::allocate(unsigned size)
	{
		free();
		data = new T[size];
		this->size = size;
	}
	
	//=========================================================================
	// Allocates new memory without losing the old data
	//=========================================================================
	template <typename T>
	void Array<T>::reallocate(unsigned size)
	{
		if(data) {
			T *data2 = new T[size];

			unsigned sizeToCopy = min<unsigned>(this->size, size);
			for(unsigned i = 0; i < sizeToCopy; i++) data2[i] = data[i];

			delete [] data;

			data = data2;
			this->size = size;
		} else {
			allocate(size);
		}
	}

	template<>
	void Array<char>::reallocate(unsigned size);

}
