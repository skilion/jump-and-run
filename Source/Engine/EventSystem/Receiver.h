#pragma once
#ifndef __RECEIVER_H__
#define __RECEIVER_H__


namespace myengine {
	
	template <class T>
	class Receiver
	{
	public:
			//Receiver();
			virtual ~Receiver();

		virtual void	onEvent(T &event) = 0;
	};


	//=========================================================================
	// Constructor
	//=========================================================================
	//template<class T>
	//Receiver<T>::Receiver() {}

	//=========================================================================
	// Destructor
	//=========================================================================
	template<class T>
	Receiver<T>::~Receiver() {}

}


#endif //!__RECEIVER_H__