#pragma once

//=========================================================================
//
//		Template for a stack (last in, first out)
//
//=========================================================================

#include "Array.h"


namespace myengine {

	template<class T>
	class Stack: Array<T>
	{
	public:
			Stack();
			Stack(unsigned size);
			Stack(const Stack<T> &stack);
			//~Stack();

		Stack<T>	&operator=(const Stack<T> &stack);

		bool		isEmpty();
		unsigned	getCount();
		
		void		push(T element);
		T			pop();
		
		void		empty();
		void		free();


	private:
		unsigned	count;
	};

	//=========================================================================
	// Constructor
	//=========================================================================
	template<class T>
	inline Stack<T>::Stack(): Array<T>()
	{
		count = 0;
	}

	template<class T>
	inline Stack<T>::Stack(unsigned size): Array<T>(size)
	{
		count = 0;
	}

	template<class T>
	inline Stack<T>::Stack(const Stack<T> &stack): Array<T>(stack)
	{
		count = stack.count;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	//template <class T>
	//Stack<T>::~Stack() {}
	
	//=========================================================================
	// Overload of operator=
	//=========================================================================
	template <typename T>
	Stack<T> &Stack<T>::operator=(const Stack<T> &stack)
	{
		count = stack.count;
		return Array<T>::operator=(stack);
	}

	//=========================================================================
	// Returns true if the stack is empty
	//=========================================================================
	template<class T>
	bool Stack<T>::isEmpty()
	{
		return 0 == count;
	}
	
	//=========================================================================
	// Returns the number of elements in the stack
	//=========================================================================
	template<class T>
	unsigned Stack<T>::getCount()
	{
		return count;
	}

	//=========================================================================
	// Pushes an element on the stack
	//=========================================================================
	template<class T>
	void Stack<T>::push(T element)
	{
		ensureSize(count + 1);
		data[count] = element;
		count++;
	}

	//=========================================================================
	// Returns and remove the element at the top of the stack
	//=========================================================================
	template<class T>
	T Stack<T>::pop()
	{
		if(count > 0)  count--;
		return data[count];
	}

	//=========================================================================
	// Removes all the elements from the stack
	//=========================================================================
	template<class T>
	void Stack<T>::empty()
	{
		count = 0;
	}

	//=========================================================================
	// Removes all the elements from the stack and frees the memory
	//=========================================================================
	template<class T>
	void Stack<T>::free()
	{
		count = 0;
		Array<T>::free();
	}
	
}
