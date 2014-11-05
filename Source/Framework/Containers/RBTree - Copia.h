#pragma once
#ifndef __RB_TREE_H__
#define __RB_TREE_H__


//=============================================================================
//
//		Template for red-black trees
//
//=============================================================================


#include "Platform.h"


namespace myengine {

	template<class T>
	class RBTree
	{
	public:
			RBTree();
			RBTree(const RBTree<T> &tree);
			~RBTree();

		RBTree<T>	&operator=(const RBTree<T> &tree);
		
		void		empty();
		T			*find(const T &element);
		void		insert(const T &element);
		void		remove(const T &element);
				
		
	private:
		class Node
		{
		public:
				Node(const T &element);
				Node(const Node *node);
				~Node();

			T		*find(const T &element);
			Node	*insert(const T &element);
			void	remove(const T &element);

			#ifdef _DEBUG
			int		check();
			#endif


		private:
			enum Color {
				COLOR_BLACK,
				COLOR_RED
			};
			
			Node	*rotateLeft();
			Node	*rotateRight();
			
			//static Node		*getSibling(Node *node);
			//static Node		*findMin(Node *node);
			
			T		element;
			Node	*left, *right;
			Color	color;
		};

		void		insert1(Node *node);
		void		insert2(Node *node, Node *parent, Node *grandparent);

		/*void		removeNode(Node *node);
		void		remove1(Node *node);
		void		remove2(Node *node);
		void		remove3(Node *node);
		void		remove4(Node *node);
		void		remove5(Node *node);
		void		remove6(Node *node);

		void		replaceNode(Node *node, Node *child);*/

		Node		*root;
	};

	
	//=========================================================================
	//
	//		Class RBTree::Node
	//
	//=========================================================================


	//=========================================================================
	// Constructor
	//=========================================================================
	template<class T>
	inline RBTree<T>::Node::Node(const T &element)
	{
		this->element = element;
		left = right = nullptr;
		color = COLOR_RED;
	}

	template<class T>
	inline RBTree<T>::Node::Node(const Node *node)
	{
		//todo
		//root = nullptr;
		//*this = a;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	template<class T>
	inline RBTree<T>::Node::~Node()
	{
		delete left;
		delete right;
	}

	//=========================================================================
	// Finds an element
	//=========================================================================
	template<class T>
	T *RBTree<T>::Node::find(const T &element)
	{
		Node *node = this;
		while(node) {
			if(element < node->element) node = node->left;
			else if(element > node->element) node = node->right;
			else return &node->element;
		}

		return nullptr;
	}
	
	//=========================================================================
	// Inserts an element
	//=========================================================================
	template<class T>
	typename RBTree<T>::Node *RBTree<T>::Node::insert(const T &element)
	{
		if(element < this->element) {
			if(left) return left->insert(element);
			return left;
		} else if(element > this->element) {
			if(right) right->insert(element);
			return right;
		} else this->element = element;

		//todo: rebalance

		return this;
	}

	//=========================================================================
	// Removes an element
	//=========================================================================
	template<class T>
	void RBTree<T>::Node::remove(const T &element)
	{
		//todo
	}

	//=========================================================================
	// Asserts if the tree is valid
	//=========================================================================
	#ifdef _DEBUG
	template<class T>
	int RBTree<T>::Node::check()
	{
		int leftHeight = 0, rightHeight = 0;

		if(left) {
			ASSERT(left->element < element);
			leftHeight = check(left);
		}

		if(right) {
			ASSERT(right->element > element);
			rightHeight = check(left);
		}
		
		ASSERT(leftHeight == rightHeight);
				
		if(Node::COLOR_RED == color) {
			if((left && Node::COLOR_RED == left->color) ||
				(right && Node::COLOR_RED == right->color))
					BREAKPOINT;
			
			return leftHeight;
		}

		return leftHeight + 1;
	}
	#endif //_DEBUG

	//=========================================================================
	// Rotates the nodes to the left and returns the new root
	//=========================================================================
	template<class T>
	inline typename RBTree<T>::Node *RBTree<T>::Node::rotateLeft()
	{
		Node *save = right;
		right = save->left;
		save->left = this;
		
		color = COLOR_RED;
		save->color = COLOR_BLACK;

		return save;
	}
	
	//=========================================================================
	// Rotates the nodes to the right and returns the new root
	//=========================================================================
	template<class T>
	inline typename RBTree<T>::Node *RBTree<T>::Node::rotateRight()
	{
		Node *save = root->left;
		root->left = save->right;
		save->right = root;
		
		color = COLOR_RED;
		save->color = COLOR_BLACK;

		return save;
	}


	//=========================================================================
	//
	//		Class RBTree
	//
	//=========================================================================
	

	//=========================================================================
	// Constructor
	//=========================================================================
	template<class T>
	inline RBTree<T>::RBTree()
	{
		root = nullptr;
	}

	template<class T>
	inline RBTree<T>::RBTree(const RBTree<T> &tree)////////////////////////////////////////
	{
		//root = nullptr;
		//*this = a;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	template<class T>
	inline RBTree<T>::~RBTree()
	{
		if(root) delete root;
	}

	//=========================================================================
	// Overload of operator=
	//=========================================================================
	template<class T>
	RBTree<T> &RBTree<T>::operator=(const RBTree<T> &tree)
	{
		//todo
		return *this;
	}

	//=========================================================================
	// Empties the tree
	//=========================================================================
	template<class T>
	inline void RBTree<T>::empty()
	{
		if(root) {
			delete root;
			root = nullptr;
		}
	}
	
	//=========================================================================
	// Finds an element
	//=========================================================================
	template<class T>
	inline T *RBTree<T>::find(const T &element)
	{
		if(root) return root->find(element);
		else return nullptr;
	}
	
	//=========================================================================
	// Inserts an element
	//=========================================================================
	template<class T>
	inline void RBTree<T>::insert(const T &element)
	{
		if(root) root = root->insert(element);
		else root = new Node(element);
	}
	
	//=========================================================================
	// Removes an element
	//=========================================================================
	template<class T>
	void RBTree<T>::remove(const T &element)
	{
		//todo
		//if(root) root->remove(element);
	}
	
	











	
	
	//=========================================================================
	// Ensures that the node does not break the rules of the red-black trees
	//=========================================================================
	template<class T>
	void RBTree<T>::insert1(typename RBTree<T>::Node *node)
	{
		Node *parent = node->parent;
		if(!parent || (Node::COLOR_BLACK == parent->color)) return;

		Node *grandparent = parent->parent;
		if(!grandparent) return;

		Node *uncle;
		if(parent == grandparent->left) uncle = grandparent->right;
		else uncle = grandparent->left;

		if(uncle && (Node::COLOR_RED == uncle->color)) {
			parent->color = Node::COLOR_BLACK;
			uncle->color = Node::COLOR_BLACK;
			if(grandparent->parent) { //Only if it isn't the root
				grandparent->color = Node::COLOR_RED;
				insert1(grandparent);
			}
		} else insert2(node, parent, grandparent);
	}

	template<class T>
	void RBTree<T>::insert2(typename RBTree<T>::Node *node, typename RBTree<T>::Node *parent, typename RBTree<T>::Node *grandparent)
	{
		if(parent->left == node) {
			if(grandparent->left == parent) {
				if(grandparent->parent) {
					parent->parent = grandparent->parent;
					if(grandparent->parent->left == grandparent)
						grandparent->parent->left = parent;
					else grandparent->parent->right = parent;
				} else {
					parent->parent = nullptr;
					root = parent;
				}

				grandparent->left = parent->right;
				if(parent->right) parent->right->parent = grandparent;
				grandparent->parent = parent;

				parent->right = grandparent;
				
				grandparent->color = Node::COLOR_RED;
				parent->color = Node::COLOR_BLACK;
			} else { //(grandparent->right == parent)
				if(grandparent->parent) {
					node->parent = grandparent->parent;
					if(grandparent->parent->left == grandparent)
						grandparent->parent->left = node;
					else grandparent->parent->right = node;
				} else {
					node->parent = nullptr;
					root = node;
				}

				if(grandparent == root) root = node;
				node->parent = grandparent->parent;

				grandparent->right = node->left;
				if(node->left) node->left->parent = grandparent;
				grandparent->parent = node;

				parent->left = node->right;
				if(node->right) node->right->parent = parent;
				parent->parent = node;

				node->left = grandparent;
				node->right = parent;

				grandparent->color = Node::COLOR_RED;
				node->color = Node::COLOR_BLACK;
			}
		} else { //if(parent->right == node) {
			if(grandparent->left == parent) {
				if(grandparent->parent) {
					node->parent = grandparent->parent;
					if(grandparent->parent->left == grandparent)
						grandparent->parent->left = node;
					else grandparent->parent->right = node;
				} else {
					node->parent = nullptr;
					root = node;
				}

				grandparent->left = node->right;
				if(node->right) node->right->parent = grandparent;
				grandparent->parent = node;

				parent->right = node->left;
				if(node->left) node->left->parent = parent;
				parent->parent = node;

				node->left = parent;
				node->right = grandparent;

				grandparent->color = Node::COLOR_RED;
				node->color = Node::COLOR_BLACK;
			} else { //(grandparent->right == parent)
				if(grandparent->parent) {
					parent->parent = grandparent->parent;
					if(grandparent->parent->left == grandparent)
						grandparent->parent->left = parent;
					else grandparent->parent->right = parent;
				} else {
					parent->parent = nullptr;
					root = parent;
				}
				

				grandparent->right = parent->left;
				if(parent->left) parent->left->parent = grandparent;
				grandparent->parent = parent;

				parent->left = grandparent;
				
				grandparent->color = Node::COLOR_RED;
				parent->color = Node::COLOR_BLACK;
			}
		}

			
			/*bool notRoot = true;
			if(!grandparent->parent) notRoot= false;

			Node *a = rotateRight(grandparent);
			if(notRoot) {
				if(grandparent == grandparent->parent->left) grandparent->parent->left = a;
				else grandparent->parent->right = a;
				a->parent = grandparent->parent;
				grandparent->parent = a;
			} else {
				root = a;
				a->parent = 0;
			}*/

		/*if((parent->right == node) && (grandparent->left == parent)) {
			rotateLeft(parent);
			grandparent = node; parent ?
			node = node->left;
		} else if((parent->left == node) && (grandparent->right == parent)) {
			rotateRight(parent);
			grandparent = node; parent ?
			node = node->right;
		}

		parent->color = Node::COLOR_BLACK;
		grandparent->color = Node::COLOR_RED;

		if((parent->left == node) && (grandparent->left == parent)) {
			ASSERT(grandparent->left == parent);
			rotateRight(parent);
		} else {	// if((parent->right == node) && (grandparent->right == parent)) {
			ASSERT(grandparent->right == parent);
			rotateLeft(parent);
		}*/
	}
	
	//=========================================================================
	// Removes the specified node
	//=========================================================================
	/*template<class T>
	void RBTree<T>::removeNode(Node *node)
	{
		Node *left = node->left;
		Node *right = node->right;
		if(left && right) {
			Node *successor = findMin(right);
			node->element = successor->element;
			removeNode(successor);

			/*Node *child = successor->right;
			replaceNode(successor, child);

			if(Node::COLOR_BLACK == node->color) {
				if(Node::COLOR_RED == child->color) {
					child->color = Node::COLOR_BLACK;
				} else remove1(child);
			}

			if(child && child->color != Node::COLOR_RED) remove1(child);
			
			delete successor;*/
		/*} else if(left) {
			replaceNode(node, left);

			if(Node::COLOR_BLACK == node->color) {
				if(Node::COLOR_RED == left->color) {
					left->color = Node::COLOR_BLACK;
				} else remove1(left);
			}

			delete node;
		} else if(right) {
			replaceNode(node, right);

			if(Node::COLOR_BLACK == node->color) {
				if(Node::COLOR_RED == right->color) {
					right->color = Node::COLOR_BLACK;
				} else remove1(right);
			}

			delete node;
		/*} else if(node->left || node->right) {
			Node *child = node->left ? node->left : node->right;

			//Replace the node with his child
			replaceNode(node, child);

			if(Node::COLOR_BLACK == node->color) {
				if(Node::COLOR_RED == child->color) {
					child->color = Node::COLOR_BLACK;
				} else remove1(child);
			}

			delete node;*/
		/*} else {	//No children
			replaceNode(node, nullptr);
			/*Node *parent = node->parent;
			if(node->parent) {BREAKPOINT; //untested code
				if(node->parent->left == node) node->parent->left = nullptr;
				else node->parent->right = nullptr;
				remove1(node->parent);
			} else {
				root = nullptr;
			}*/
			/*delete node;
		}
	}
	
	//=========================================================================
	// Ensures that the node does not break the rules of the red-black trees
	//=========================================================================
	template<class T>
	void RBTree<T>::remove1(typename RBTree<T>::Node *node)
	{
		if(node->parent != nullptr) remove2(node);
	}

	template<class T>
	void RBTree<T>::remove2(typename RBTree<T>::Node *node)
	{
		Node *sibling = getSibling(node);
 
		if(Node::COLOR_RED == sibling->color) {
			node->parent->color = Node::COLOR_RED;
			sibling->color = Node::COLOR_BLACK;
			if(node == node->parent->left) rotateLeft(node->parent);
			else rotateRight(node->parent);
		} 
		remove3(node);
	}

	template<class T>
	void RBTree<T>::remove3(typename RBTree<T>::Node *node)
	{
		Node *sibling = getSibling(node);
 
		if((Node::COLOR_BLACK == node->parent->color) &&
			(Node::COLOR_BLACK == sibling->color) &&
			(Node::COLOR_BLACK == sibling->left->color) &&
			(Node::COLOR_BLACK == sibling->right->color)) {
				sibling->color = Node::COLOR_RED;
				remove1(node->parent);
		} else remove4(node);
	}

	template<class T>
	void RBTree<T>::remove4(typename RBTree<T>::Node *node)
	{
		Node *sibling = getSibling(node);
		
		if((Node::COLOR_RED == node->parent->color) &&
			(Node::COLOR_BLACK == sibling->color) &&
			(Node::COLOR_BLACK == sibling->left->color) &&
			(Node::COLOR_BLACK == sibling->right->color)) {
				sibling->color = Node::COLOR_RED;
				node->parent->color = Node::COLOR_BLACK;
        } else remove5(node);
	}

	template<class T>
	void RBTree<T>::remove5(typename RBTree<T>::Node *node)
	{
		Node *sibling = getSibling(node);
		
		ASSERT(sibling->color == Node::COLOR_BLACK);
		
		if((node == node->parent->left) && (Node::COLOR_BLACK == sibling->right->color)) {
				ASSERT(Node::COLOR_RED == sibling->left->color)
				sibling->color = Node::COLOR_RED;
				sibling->left->color = Node::COLOR_BLACK;
				rotateRight(sibling);
		} else if((node == node->parent->right) &&
			(Node::COLOR_BLACK == sibling->left->color)) {
			ASSERT(Node::COLOR_RED == sibling->right->color);
			sibling->color = Node::COLOR_RED;
			sibling->right->color = Node::COLOR_BLACK;
			rotateLeft(sibling);
		}
		remove6(node);
	}

	template<class T>
	void RBTree<T>::remove6(typename RBTree<T>::Node *node)
	{
		Node *sibling = getSibling(node);
		
		sibling->color = node->parent->color;
		node->parent->color =  Node::COLOR_BLACK;
 
		if (node == node->parent->left) {
			sibling->right->color =  Node::COLOR_BLACK;
			rotateLeft(node->parent);
		} else {
			sibling->left->color =  Node::COLOR_BLACK;
			rotateRight(node->parent);
		}
	}
	
	//=========================================================================
	// Replaces a node with his child
	//=========================================================================
	template<class T>
	inline void RBTree<T>::replaceNode(typename RBTree<T>::Node *node, typename RBTree<T>::Node *child)
	{
		Node *parent = node->parent;
		if(parent) {
			if(parent->left == node) parent->left = child;
			else parent->right = child;
		} else root = child;

		if(child) child->parent = parent;
	}*/
	
	





	//=========================================================================
	// Returns the left-most child of a node
	//=========================================================================
	/*template<class T>
	inline typename RBTree<T>::Node *RBTree<T>::findMin(typename RBTree<T>::Node *node)
	{
		Node *min = node;
		while(min->left) min = min->left;
		return min;
	}*/

}


#endif //!__RB_TREE_H__
