#pragma once
#ifndef __SENDER_H__
#define __SENDER_H__


#include "Receiver.h"

#include "Containers/List.h"


namespace myengine {
		
	template<typename T>
	class Sender
	{
	public:
			//Sender();
			//~Sender();

		void	connect(Receiver<T> *receiver);
		void	disconnect(Receiver<T> *receiver);

		void	sendEvent(T &event);


	private:
		List<Receiver<T>*>	receivers;
	};

	
	//=========================================================================
	// Constructor
	//=========================================================================
	//template<typename T>
	//Sender<T>::Sender() {}

	//=========================================================================
	// Destructor
	//=========================================================================
	//template<typename T>
	//Sender<T>::~Sender() {}

	//=========================================================================
	// Connects a receiver
	//=========================================================================
	template<typename T>
	void Sender<T>::connect(Receiver<T> *receiver)
	{
		receivers.addFirst(receiver);
	}

	//=========================================================================
	// Disconnects a receiver
	//=========================================================================
	template<typename T>
	void Sender<T>::disconnect(Receiver<T> *receiver)
	{
		const typename List< Receiver<T>*>::Node *node = receivers.getHead();
		while(node) {
			if(node->element == receiver) {
				receivers.remove(node);
				break;
			}

			node = node->next;
		}
	}

	//=========================================================================
	// Sends an event to all receivers
	//=========================================================================
	template<typename T>
	void Sender<T>::sendEvent(T &event)
	{
		const typename List<Receiver<T>*>::Node *node = receivers.getHead();
		while(node) {
			node->element->onEvent(event);
			node = node->next;
		}
	}

}


#endif //!__SENDER_H__