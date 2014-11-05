#pragma once

#include "Platform.h"


namespace myengine {

	template<class T>
	class List
	{
	public:
		struct Node {
			Node	*next, *prev;
			T		element;
		};

			List();
			~List();

		void		addFirst(const T &element);
		void		addLast(const T &element);
		void		add(Node *node, const T &element);

		bool		contains(const T &element);

		void		free();

		const Node	*getHead() const;
		const Node	*getTail() const;

		T			getFirst() const;
		T			getLast() const;

		bool		isEmpty() const;

		void		remove(const T &element);
		void		remove(const Node *node);
		void		removeFirst();
		void		removeLast();


	private:
		Node	*head, *tail;
	};

	//=========================================================================
	// Constructor
	//=========================================================================
	template<class T>
	List<T>::List()
	{
		head = tail = nullptr;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	template <class T>
	List<T>::~List()
	{
		free();
	}

	//=========================================================================
	// Inserts the given element at the beginning of the list
	//=========================================================================
	template<class T>
	void List<T>::addFirst(const T &element)
	{
		Node *node = new Node;
		node->prev = nullptr;
		node->next = head;
		node->element = element;

		if(head) {
			head->prev = node;
			head = node;
		} else {
			head = tail = node;
		}
	}

	//=========================================================================
	// Appends the given element to the end of the list
	//=========================================================================
	template<class T>
	inline void List<T>::addLast(const T &element)
	{
		add(tail, element);
		/*Node *node = new Node;
		node->prev = tail;
		node->next = nullptr;
		node->element = element;

		if(tail) {
			tail->next = node;
			tail = node;
		} else {
			head = tail = node;
		}*/
	}

	//=========================================================================
	// Insert the element specified after the node specified
	//=========================================================================
	template<class T>
	void List<T>::add(Node *position, const T &element)
	{
		if(position) {
			Node *node = new Node;
			Node *next = position->next;
			node->element = element;
			node->prev = position;
			node->next = next;

			if(next) next->prev = node;
			else /*if(position == tail)*/ tail = node;
		}
	}

	//=========================================================================
	// Returns true if the list contains the specified element
	//=========================================================================
	template<class T>
	bool List<T>::contains(const T &element)
	{
		const Node *node = head;
		while(node) {
			if(node->element == element) {
				return true;
			}
			node = node->next;
		}

		return false;
	}

	//=========================================================================
	// Removes all the elements from the list
	//=========================================================================
	template<class T>
	void List<T>::free()
	{
		const Node *node = head, *tmp;
		while(node) {
			tmp = node;
			node = node->next;
			delete tmp;
		}

		head = tail = nullptr;
	}

	//=========================================================================
	// Returns the head node
	//=========================================================================
	template<class T>
	const typename List<T>::Node *List<T>::getHead() const
	{
		return head;
	}

	//=========================================================================
	// Returns the tail node
	//=========================================================================
	template<class T>
	const typename List<T>::Node *List<T>::getTail() const
	{
		return tail;
	}

	//=========================================================================
	// Returns the first element in the list
	//=========================================================================
	template<class T>
	T List<T>::getFirst() const
	{
		//ASSERT(head);
		return head->element;
	}

	//=========================================================================
	// Returns the last element in the list
	//=========================================================================
	template<class T>
	inline T List<T>::getLast() const
	{
		ASSERT(tail);
		return tail->element;
	}

	//=========================================================================
	// Returns true if the list is empty
	//=========================================================================
	template<class T>
	inline bool List<T>::isEmpty() const
	{
		return head == /* tail == */ nullptr;
	}

	//=========================================================================
	// Removes the first occurrence of the specified element in the list
	//=========================================================================
	template<class T>
	void List<T>::remove(const T &element)
	{
		Node *node = head;
		while(node) {
			if(node->element == element) {
				remove(node);
				break;
			}
			node = node->next;
		}
	}

	//=========================================================================
	// Removes the specified node from the list
	//=========================================================================
	template<class T>
	void List<T>::remove(const Node *node)
	{
		if(node == head) removeFirst();
		else if(node == tail) removeLast();
		else {
			Node *next = node->next;
			Node *prev = node->prev;

			/*if(next)*/ next->prev = prev;
			/*if(prev)*/ prev->next = next;

			delete node;
		}
	}

	//=========================================================================
	// Removes the first element in the list
	//=========================================================================
	template<class T>
	void List<T>::removeFirst()
	{
		if(head == tail) {
			delete head;
			head = tail = nullptr;
		} else {
			Node *tmp = head;
			head = head->next;
			head->prev = nullptr;
			delete tmp;
		}
	}

	//=========================================================================
	// Removes the first element in the list
	//=========================================================================
	template<class T>
	void List<T>::removeLast()
	{
		if(tail == head) {
			delete tail;
			tail = head = nullptr;
		} else {
			Node *tmp = tail;
			tail = tail->prev;
			tail->next = nullptr;
			delete tmp;
		}
	}

}
