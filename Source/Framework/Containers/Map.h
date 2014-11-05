#pragma once
#ifndef __MAP_H__
#define __MAP_H__


#include "Array.h"


namespace myengine {
	
	template<class K, class T> //Must support operators < and >
	class Map
	{
	public:
			Map();
			Map(unsigned size);
			Map(const Map<K, T> &map);
			//~Map();

		Map<K, T>	&operator=(const Map<K, T> &map);

		T			&operator[](const K &key);
		
		bool		find(const K &key, T &value);

		void		insert(const K &key, const T &value);

		void		remove(const K &key);


	private:
		struct Element {
			K	key;
			T	value;
		};

		bool	find(const K &key, unsigned &index);
		void	insert(Element *element);

		unsigned		count;
		Array<Element*>	data;
	};


	//=========================================================================
	// Constructor
	//=========================================================================
	template<class K, class T>
	Map<K, T>::Map()
	{
		count = 0;
	}

	template<class K, class T>
	Map<K, T>::Map(unsigned size): elements(size)
	{
		Map();
	}

	template<class K, class T>
	Map<K, T>::Map(const Map<K, T> &map)
	{
		Map();
		*this = map;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	//template<class K, class T>
	//Map<T>::~Map() {}
	
	//=========================================================================
	// Overload of operator=
	//=========================================================================
	template<class K, class T>
	Map<K, T> &Map<K, T>::operator=(const Map<K, T> &map) //todo: test
	{
		count = map.count;
		data.ensureSize(count);
		
		for(unsigned i = 0; i < count; i++) {
			Element *element = new Element;
			element->key = map.data[i]->key;
			element->value = map.data[i]->value;
			insert(element);
		}

		return *this;
	}

	//=========================================================================
	// Overload of operator[]
	//=========================================================================
	template<class K, class T>
	T &Map<K, T>::operator[](const K &key)
	{
		unsigned index;
		if(!find(key, index)) {
			Element *element = new Element;
			element->key = key;
			element->value = T();

			insert(element);

			return &element->value;
		}

		return &data[index]->value;
	}
	
	//=========================================================================
	// Finds an element and returns its value
	//=========================================================================
	template<class K, class T>
	bool Map<K, T>::find(const K &key, T &value)
	{
		unsigned index;
		if(!find(key, index)) {
			return false;
		}

		value = data[i]->value;

		return true;
	}

	//=========================================================================
	// Inserts an element
	//=========================================================================
	template<class K, class T>
	void Map<K, T>::insert(const K &key, const T &value)
	{
		Element *element = /*new Element*/0;
		element->key = key;
		element->value = value;

		insert(element);
	}
	
	//=========================================================================
	// Removes an element
	//=========================================================================
	template<class K, class T>
	void Map<K, T>::remove(const K &key)
	{
		unsigned index;
		if(find(key, index)) {
			delete data[i];
			//todo: translate elements
			count--;
		}
	}
	
	//=========================================================================
	// Finds an element and returns its index
	//=========================================================================
	template<class K, class T>
	bool Map<K, T>::find(const K &key, unsigned &index)
	{
		unsigned x1 = 0, x2 = count - 1, m;
	
		while(x1 <= x2) {
			m = (x1 + x2) / 2;
			if(data[m]->key < key) x1 = m + 1;
			else if(data[m]->key > key) x2 = m - 1;
			else {
				index = m;
				return true;
			}
		}

		return false;
	}
	
	//=========================================================================
	// Inserts an element
	//=========================================================================
	template<class K, class T>
	void Map<K, T>::insert(Element *element)
	{
		data.ensureSize(count + 1);

		bool inserted = false;
		for(unsigned i = 0; i < count; i++) {
			if(element->key < data[i]->key) {
				//todo: translate elements
				data[i] = element;
				inserted = true;
				break;
			}
		}

		if(!inserted) data[count] = element;

		count++;
	}

}


#endif //!__MAP_H__