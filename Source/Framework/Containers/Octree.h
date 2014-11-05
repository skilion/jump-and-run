#pragma once
#ifndef __OCTREE_H__
#define __OCTREE_H__


//=============================================================================
//
//		Template for octrees
//
//=============================================================================


#include "Math/Math.h"


namespace myengine {
	
	template<class T>
	class Octree
	{
	public:
			Octree(unsigned size);
			~Octree();

		T		*get(unsigned x, unsigned y, unsigned z) const;
		void	insert(unsigned x, unsigned y, unsigned z, T *element);
		void	remove(unsigned x, unsigned y, unsigned z);

		void	free();


	private:
		unsigned	getIndex(unsigned x, unsigned y, unsigned z) const;

		unsigned	size;
		
		union {
			Octree	*nodes[8];
			T		*leaves[8];	//size == 1
		};
	};


	//=========================================================================
	// Constructor
	//=========================================================================
	template<class T>
	Octree<T>::Octree(unsigned size)
	{
		if(!isPow2(size)) throw;
		this->size = size;

		for(int i = 0; i < 8; i++) nodes[i] = nullptr;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	template<class T>
	inline Octree<T>::~Octree()
	{
		free();
	}
	
	//=========================================================================
	// Returns the element at the specified position
	//=========================================================================
	template<class T>
	T *Octree<T>::get(unsigned x, unsigned y, unsigned z) const
	{
		if(size == 1) {
			return leaves[(x & 1) + ((y & 1) << 1) + ((z & 1) << 2)];
		} else {
			unsigned index = getIndex(x, y, z);
			Octree<T> *node = nodes[index];
			if(node) return node->get(x, y, z);
		}

		return nullptr;
	}
	
	//=========================================================================
	// Inserts an element at the specified position
	//=========================================================================
	template<class T>
	void Octree<T>::insert(unsigned x, unsigned y, unsigned z, T *element)
	{
		if(!element) throw;

		if(size == 1) {
			unsigned index = (x & 1) + ((y & 1) << 1) + ((z & 1) << 2);
			T *leaf = leaves[index];
			if(leaf) delete leaf;
			leaves[index] = element;
		} else {
			unsigned index = getIndex(x, y, z);
			Octree<T> *node = nodes[index];

			if(!node) node = nodes[index] = new Octree<T>(size >> 1);
			node->insert(x, y, z, element);
		}
	}
	
	//=========================================================================
	// Removes an element at the specified position
	//=========================================================================
	template<class T>
	void Octree<T>::remove(unsigned x, unsigned y, unsigned z)
	{
		if(size == 1) {
			unsigned index = (x & 1) + ((y & 1) << 1) + ((z & 1) << 2);
			delete leaves[index];
			leaves[index] = nullptr;
		} else {
			unsigned index = getIndex(x, y, z);
			delete nodes[index];
			nodes[index] = nullptr;
		}
	}
	
	//=========================================================================
	// Frees the memory
	//=========================================================================
	template<class T>
	void Octree<T>::free()
	{
		if(size == 1) {
			for(int i = 0; i < 8; i++) {
				delete nodes[i];
				nodes[i] = nullptr;
			}
		} else {
			for(int i = 0; i < 8; i++) {
				delete leaves[i];
				leaves[i] = nullptr;
			}
		}
	}
	
	//=========================================================================
	// Returns the index according to the current size
	//=========================================================================
	template<class T>
	inline unsigned Octree<T>::getIndex(unsigned x, unsigned y, unsigned z) const
	{
		unsigned a = (x & size) != 0;
		unsigned b = (y & size) != 0;
		unsigned c = (z & size) != 0;
		return a + (b << 1) + (c << 2);
	}

}


#endif //!__OCTREE_H__