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

		struct Node;

#ifdef _DEBUG
		int			check(Node *root = 0);
#endif
		
		
	private:
		struct Node {
			enum Color {
				COLOR_RED,
				COLOR_BLACK
			};
			
			T		element;
			//Node	*parent;
			Node	*left, *right;
			Color	color;
		};

		static void		*rotateLeft(Node &root);
		static void		*rotateRight(Node &root);

		//void	insert1(Node *node);
		//void	insert2(Node *node, Node *parent, Node *grandparent);
		
		//void	rotateLeft(Node *root);
		//void	rotateRight(Node *root);

		/*void		removeNode(Node *node);
		void		remove1(Node *node);
		void		remove2(Node *node);
		void		remove3(Node *node);
		void		remove4(Node *node);
		void		remove5(Node *node);
		void		remove6(Node *node);

		void		replaceNode(Node *node, Node *child);*/

		Node	*root;

		//static Node		*findMin(Node *node);
		//static Node		*getSibling(Node *node);
		
	};
	

	//=========================================================================
	// Constructor
	//=========================================================================
	template<class T>
	inline RBTree<T>::RBTree()
	{
		root = nullptr;
	}

	template<class T>
	inline RBTree<T>::RBTree(const RBTree<T> &tree)
	{
		root = nullptr;
		*this = a;
	}

	//=========================================================================
	// Destructor
	//=========================================================================
	template<class T>
	inline RBTree<T>::~RBTree()
	{
		delete root;
	}

	//=========================================================================
	// Overload of operator=
	//=========================================================================
	template<class T>
	RBTree<T> &RBTree<T>::operator=(const RBTree<T> &tree)
	{
		return *this;/////////////
	}

	//=========================================================================
	// Empties the tree
	//=========================================================================
	template<class T>
	void RBTree<T>::empty()
	{
		Node *node = root, tmp;
		while(node) {
			if(node->left) node = node->left;
			else if(node->right) node = node->right;
			else {
				tmp = node;
				node = node->parent;
				delete tmp;
			}
		}

		root = nullptr;
	}
	
	//=========================================================================
	// Finds an element
	//=========================================================================
	template<class T>
	T *RBTree<T>::find(const T &element)
	{
		Node *node = root;
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
	void RBTree<T>::insert(const T &element)
	{
		Node head;
		Node *grandparent;
		Node *parent = &head;
		Node *i = root;
		node->color = Node::COLOR_RED;

		

		{
			if(i) {
			} else {
				Node *node = new Node;
				node->element = element;
				node->left = node->right = nullptr;
			}
				if(element < parent->element) {
					if(parent->left) i = parent->left;
					else {
						parent->left = node;
						break;
					}
				} else if(element > parent->element) {
					if(parent->right) i = parent->right;
					else {
						parent->right = node;
						break;
					}
				} else { //Overwrite current element
					parent->element = element;
					return;
				}

		}

		root->color = Node::COLOR_BLACK;

		/*if(!root) { //First node
			node->parent = nullptr;
			node->color = Node::COLOR_BLACK;
			root = node;
		} else {
			Node *parent = root;
			
			while(true) {
				if(element < parent->element) {
					if(parent->left) parent = parent->left;
					else {
						parent->left = node;
						break;
					}
				} else if(element > parent->element) {
					if(parent->right) parent = parent->right;
					else {
						parent->right = node;
						break;
					}
				} else { //Overwrite current element
					parent->element = element;
					return;
				}
			}

			node->parent = parent;
			node->color = Node::COLOR_RED;

			insert1(node);
		}*/
		
	}
	
	//=========================================================================
	// Removes an element
	//=========================================================================
	template<class T>
	void RBTree<T>::remove(const T &element)
	{
		/*Node *node = root;
		while(node) {
			if(element < node->element) node = node->left;
			else if(element > node->element) node = node->right;
			else {
				removeNode(node);
				break;
			}
		}*/
	}
	
	//=========================================================================
	// Checks if the tree is valid
	//=========================================================================
#ifdef _DEBUG
	template<class T>
	int RBTree<T>::check(typename RBTree<T>::Node *root)
	{
		if(!root) root = this->root;
		
		Node *left = root->left;
		Node *right = root->right;

		if(left && left->parent != root) BREAKPOINT;
		if(right && right->parent != root) BREAKPOINT;
		
		if(Node::COLOR_RED == root->color) {
			if((left && Node::COLOR_RED == left->color) ||
				(right && Node::COLOR_RED == right->color))
					BREAKPOINT;
		}
		
		if((left && left->element >= root->element) ||
			(right && right->element <= root->element))
				BREAKPOINT;
		
		int leftHeight = left ? check(left) : 0;
		int rightHeight = right ? check(right) : 0;
		
		if(leftHeight != rightHeight) BREAKPOINT;

		if(Node::COLOR_RED == root->color) return leftHeight;
		return leftHeight + 1;
	}
#endif //_DEBUG

	//=========================================================================
	// Rotates the nodes to the left
	//=========================================================================
	/*template<class T>
	inline void RBTree<T>::rotateLeft(typename RBTree<T>::Node *root)
	{
		Node *newRoot = root->right;	
		root->right = newRoot->left;
		newRoot->left = root;
		root = newRoot;
	}
	
	//=========================================================================
	// Rotates the nodes to the right
	//=========================================================================
	template<class T>
	inline void RBTree<T>::rotateRight(typename RBTree<T>::Node* &root)
	{
		Node *newRoot = root->left;	
		root->left = newRoot->right;
		newRoot->right = root;
		root = newRoot;
	}*/









	
	
	//=========================================================================
	// Ensures that the node does not break the rules of the red-black trees
	//=========================================================================
	/*template<class T>
	void RBTree<T>::insert1(typename RBTree<T>::Node *node)
	{
		Node *parent = node->parent;
		if(Node::COLOR_BLACK == parent->color) return;

		Node *grandparent = parent->parent;
		//if(!grandparent) return;
		ASSERT(grandparent);

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
			if(grandparent->left == parent) rotateRight(grandparent);
			else { //(grandparent->right == parent)
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
			} else rotateLeft(grandparent); //(grandparent->right == parent)
		}
	}*/

	//=========================================================================
	// Rotates the nodes to the left and returns the new root
	//=========================================================================
	/*template<class T>
	inline void RBTree<T>::rotateLeft(typename RBTree<T>::Node *root)
	{
		Node *newRoot = root->right;

		if(root->parent) {
			Node *rootParent = root->parent;
			if(rootParent->left == root)
				rootParent->left = newRoot;
			else rootParent->right = newRoot;
			newRoot->parent = rootParent;
		} else {
			this->root = newRoot;
			newRoot->parent = nullptr;
		}
		
		root->parent = newRoot;		
		root->right = newRoot->left;
		if(newRoot->left) newRoot->left->parent = root;
		root->color = Node::COLOR_RED;

		newRoot->left = root;
		newRoot->color = Node::COLOR_BLACK;
	}
	
	//=========================================================================
	// Rotates the nodes to the right and returns the new root
	//=========================================================================
	template<class T>
	inline void RBTree<T>::rotateRight(typename RBTree<T>::Node *root)
	{
		Node *newRoot = root->left;

		if(root->parent) {
			Node *rootParent = root->parent;
			if(rootParent->left == root)
				rootParent->left = newRoot;
			else rootParent->right = newRoot;
			newRoot->parent = rootParent;
		} else {
			this->root = newRoot;
			newRoot->parent = nullptr;
		}
		
		root->parent = newRoot;		
		root->left = newRoot->right;
		if(newRoot->right) newRoot->right->parent = root;
		root->color = Node::COLOR_RED;

		newRoot->right = root;
		newRoot->color = Node::COLOR_BLACK;
	}*/
	
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
