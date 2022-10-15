/* 

Student Info
------------
Name : Taras Ermolenko 
St.# : 301448503
Email: taras.ermolenko@sfu.ca
Date : 2022-03-25

Program Details
---------------
CMTP 225 Assingment 5

Statement of Originality
------------------------
All the code and comments below are my own original work. For any non-
original work, I have provided citations in the comments with enough detail so
that someone can see the exact source and extent of the borrowed work.
In addition, I have not shared this work with anyone else, and I have not seen
solutions from other students, tutors, websites, books, etc.
etc.

*/

#include <iostream>
#include <vector>
#include <stdexcept>

#pragma once

template <class T, class Y>
class NodeT
{
public:

    NodeT(T nodeKey, Y nodeVal);

    NodeT(T nodeKey, Y nodeVal, NodeT<T, Y>* nodeParent, bool color);

    T key;

    Y value;

    NodeT<T,Y>* left;

    NodeT<T,Y>* right;

    NodeT<T,Y>* parent;

    bool isBlack;

};

template <class T, class Y>
class RedBlackTree
{
public:

    RedBlackTree<T,Y>();

    ~RedBlackTree<T,Y>();

    RedBlackTree(RedBlackTree<T,Y> &copy);

    RedBlackTree<T,Y>& operator=(const RedBlackTree<T,Y> &copy);

    bool insert(T key, Y value);
    
    bool remove(T key);

    bool search(T key) const;

    std::vector<Y> search(T fromKey, T toKey) const;

    std::vector<Y> values() const;

    std::vector<T> keys() const;

    int size() const;

    NodeT<T,Y>* getRoot() const;

private:

    NodeT<T,Y>* root;

    int tree_size;
    
    // Helper Fucntions
    NodeT<T,Y>* clone(NodeT<T,Y>* t) const;

    bool empty() const;

    void clear();

    void clear(NodeT<T,Y>* nd);

    NodeT<T,Y>* bstInsert(T key, Y value);

    bool isBlack(NodeT<T,Y>* nd);

    void leftRotate(NodeT<T,Y>* x);

    void rightRotate(NodeT<T,Y>* x);

    bool bstRemove(T key);

    bool bstRemove(NodeT<T,Y>* nd);

    NodeT<T,Y>* findNode(T key, NodeT<T,Y>* & parent) const;

    NodeT<T,Y>* findNode(T key) const;

    NodeT<T,Y>* getSuccessor(NodeT<T,Y>* target, NodeT<T,Y>* & sParent) const;

    NodeT<T,Y>* getPredecessor(NodeT<T,Y>* target, NodeT<T,Y>* & sParent) const;

    void rbFix(NodeT<T,Y>* x, NodeT<T,Y>* parent, bool rightChild);

    void searchInOrderTraversalCall(NodeT<T,Y>* node, std::vector<Y> &vec, T fromKey, T toKey) const;

    void valuesInOrderTraversalCall(NodeT<T,Y>* node, std::vector<Y> &vec) const; 

    void keysInOrderTraversalCall(NodeT<T,Y>* node, std::vector<T> &vec) const; 


};

// RedBlackTree Methods implementations

// Default Node Constructor
template <class T, class Y>
NodeT<T,Y>::NodeT(T nodeKey, Y nodeVal)
{
    key = nodeKey;
    value = nodeVal;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    isBlack = false;
}

// Overloaded node constructor with parent pointer
template <class T, class Y>
NodeT<T,Y>::NodeT(T nodeKey, Y nodeVal, NodeT<T,Y>* nodeParent, bool color)
{
    key = nodeKey;
    value = nodeVal;
    parent = nodeParent;
    left = nullptr;
    right = nullptr;
    isBlack = color;
}


// is node black method that check for not nullpointer aswell (x not nullptr and not black color == red)
template <class T, class Y>
bool RedBlackTree<T,Y>::isBlack(NodeT<T,Y>* nd)
{
    if(nd != nullptr && nd->isBlack == false)
    {
        return false;
    }
    else
    {
        return true;
    }

}

// RedBlackTree constructor
template <class T, class Y>
RedBlackTree<T,Y>::RedBlackTree()
{
    root = nullptr;
    tree_size = 0;
};

// RedBlackTree destructor
template <class T, class Y>
RedBlackTree<T,Y>::~RedBlackTree()
{
    clear();
};

// RedBlackTree copy constructor
template <class T, class Y>
RedBlackTree<T,Y>::RedBlackTree(RedBlackTree<T,Y> &copy)
{
    tree_size = copy.tree_size;
    // copy tree
	root = clone(copy.root);
};

// RedBlackTree overloaded assignment operator
template <class T, class Y>
RedBlackTree<T,Y>& RedBlackTree<T,Y>::operator=(const RedBlackTree<T,Y> &copy)
{
	// check that not copying itself
	if(this != &copy)
	{
		// if not empty free memeory
		if(!empty())
		{
			clear();
		} 

		//clone tree
		root = clone(copy.root);
		tree_size = copy.tree_size;
	}

	return *this;
}

// PRE: recursive binary tree clone helper method
// PARAM: root node of tree you want to copy
// POST: returns a pointer to a new copy of a tree starting from root you passed as a parameter
template <class T, class Y>
NodeT<T,Y>* RedBlackTree<T,Y>::clone(NodeT<T,Y>* t) const
{
	// base case when reaching nullptr return
	if(t == nullptr)
	{
		return nullptr;
	}

	// create new tree node copy to insert
	NodeT<T,Y>* copy = new NodeT<T,Y>(t->key, t->value, t->parent, t->isBlack);

	//recursivley move down left and right of binary tree
	copy->left = clone(t->left);
	copy->right = clone(t->right);

	// returns root node of copied tree once all calls reach base case
	return copy;

}

// returns pointer to the root of the tree
template <class T, class Y>
NodeT<T,Y>* RedBlackTree<T,Y>::getRoot() const
{
    return root;
};

// returns true if tree is empty, false if not
template <class T, class Y>
bool RedBlackTree<T,Y>::empty() const
{
	return root == nullptr;
}

// call clear helper method wchich frees tree memory of all nodes
template <class T, class Y>
void RedBlackTree<T,Y>::clear()
{
	clear(root);
	root = nullptr;
}

// Recursivley goes down tree and deletes all the nodes freeing memory.
template <class T, class Y>
void RedBlackTree<T,Y>::clear(NodeT<T,Y>* nd)
{
	if (nd != nullptr) {
		clear(nd->left);
		clear(nd->right);
		delete nd;
	}
}

// PRE: assuming enough memeory in system to insert a value
// PARAM: key you want to insert, value you want to be attached to that key
// POST: balanced tree with new node with given key
template <class T, class Y>
bool RedBlackTree<T,Y>::insert(T key, Y value)
{
    // make sure key not already in tree
    if(search(key))
    {
        return false;
    }

    // Insert node
    NodeT<T,Y>* x = bstInsert(key, value);
    
    // update count
    tree_size++;

    // new node is red
    x->isBlack = false;
    
    // Fix color
    
    while(x != root && isBlack(x->parent) == false) // iterate until the root or a black parent is reached
    {
        if(x->parent == x->parent->parent->left) // x's parent is a left child
        {
            NodeT<T,Y>* y = x->parent->parent->right; // "uncle" of x
            if(isBlack(y) == false) // if uncle is red
            {
                x->parent->isBlack = true;
                y->isBlack = true;
                x->parent->parent->isBlack = false;
                x = x->parent->parent;
            }
            else // y.color (uncle of x) == black
            {
                if(x == x->parent->right)
                {
                    x = x->parent;
                    leftRotate(x);
                    
                }
                x->parent->isBlack = true;
                x->parent->parent->isBlack = false;
                rightRotate(x->parent->parent);
            }
        }
        else // if x's parent is a right child
        {
            NodeT<T,Y>* y = x->parent->parent->left; 
            if(isBlack(y) == false) // if uncle of x is red
            {
                x->parent->isBlack = true;
                y->isBlack = true;
                x->parent->parent->isBlack = false;
                x = x->parent->parent;
            }
            else    // if uncle of x is black 
            {
                if(x == x->parent->left)
                {
                    x = x->parent;
                    rightRotate(x);
                }
                x->parent->isBlack = true;
                x->parent->parent->isBlack = false;
                leftRotate(x->parent->parent);
            }
        }
    }
    root->isBlack = true;
    return true;
};


// preforms left roattion on given node
template <class T, class Y>
void RedBlackTree<T,Y>::leftRotate(NodeT<T,Y>* x)
{
    NodeT<T,Y>* y = x->right;
    x->right = y->left;
    // Set nodes' parents references
    // y's left child
    if(y->left != nullptr)
    {
        y->left->parent = x;
    }

    //y
    y->parent = x->parent;

    // Set child reference of nd's parent
    if(x->parent == nullptr) // node was root
    {
        root = y;
    }
    else if(x == x->parent->left) // left child
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    // Make nd y's left child
    y->left = x;
    x->parent = y;
}

// preforms right roattion on given node
template <class T, class Y>
void RedBlackTree<T,Y>::rightRotate(NodeT<T,Y>* x)
{
    NodeT<T,Y>* y = x->left;
    x->left = y->right;
    // Set nodes' parents references

    if (y->right != nullptr) 
    {
      y->right->parent = x;
    }

    y->parent = x->parent;

    // Set child reference of nd's parent
    if (x->parent == nullptr) 
    {
      root = y;
    }
    else if (x == x->parent->right) 
    {
      x->parent->right = y;
    } 
    else 
    {
      x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
};

// PRE: Binary Tree insert 
// PARAM: nodey key and value
// POST: Node inserted into binary tree (may be unbalanced)
template <class T, class Y>
NodeT<T,Y>* RedBlackTree<T,Y>::bstInsert(T key, Y value)
{
	NodeT<T,Y>* newNode = new NodeT<T,Y>(key, value);
	NodeT<T,Y>* parent = root;
    // insertion point
	NodeT<T,Y>* next = root;

	if (empty())
    {
		root = newNode;
	}
	else 
    { //non-empty
		   // Find parent of new node
		while (next != nullptr) 
        {
			parent = next;
			if (key < parent->key) 
            {
				next = parent->left;
			}
			else 
            {
				next = parent->right;
			}
		}
		// Insert new node
		if (key < parent->key) 
        {
			parent->left = newNode;
		}
		else 
        {
			parent->right = newNode;
		}
	}
    
    // set parent of new node
    newNode->parent = parent;

    return newNode;
}


// PRE: 
// PARAM: key of node you want to remove
// POST: tree without node specified and balanced
template <class T, class Y>
bool RedBlackTree<T,Y>::remove(T key)
{
    NodeT<T,Y>* parent;
    NodeT<T,Y>* z = findNode(key, parent);

    if(z == nullptr)
    {
        return false;
    }

    // node that is going to be removed
    NodeT<T,Y>* y;
    // Parent of removed node also becomes x's parent
    NodeT<T,Y>* yParent;

    // child of removed node (what to call rbfix on) can be null
    NodeT<T,Y>* x;


    bool isRightChild = true;


    if(z == nullptr)
    {
        return false;
    }

    if(z->left == nullptr || z->right == nullptr)
    {
        y = z; // node to be removed
        yParent = y->parent;
    }
    else
    {
        y = getSuccessor(z, yParent);
    }
    if(y->left != nullptr)
    {
        x = y->left;
    }
    else
    {
        x = y->right;
    }
    if(x != nullptr)
    {
        x->parent = y->parent; // detach x from y if not null
    }
    if(y->parent == nullptr)
    {
        root = x;
    }
    else
    {
        // attach x to y's parent
        if(y == y->parent->left) // left child
        {
            y->parent->left = x;
            isRightChild = false;
        }
        else
        {
            y->parent->right = x;
            isRightChild = true;
        }
    }

    if (y != z)
    {
        z->value = y->value;
        z->key = y->key;
    }

    bool isRemovedBlack = isBlack(y);
    delete y;

    if(isRemovedBlack)
    {
        // pass x, parent of x and if x is a right child
        rbFix(x, yParent, isRightChild);
    }

    tree_size--;
    return true;
}



// Searches tree for target node
// PRE: overloaded with reference to parent node
// PARAM: key = key to be found, reference to parent node
// POST: Returns pointer to node if found, nullptr if not found.
template <class T, class Y>
NodeT<T,Y>* RedBlackTree<T,Y>::findNode(T key, NodeT<T,Y>* & parent) const
{
	NodeT<T,Y>* current = root;
    parent = nullptr;
    // loops until found or nullptr reached
	while (current != nullptr) 
    {
		if (key == current->key) 
        {
			return current;
		}
        parent = current;
		if (key < current->key) 
        {
			current = current->left;
		}
		else { // key > current->data
			current = current->right;
		}
	}
    // if does not find value will go down to and return nullptr
	return current;
}

// Searches tree for target node
// PRE:
// PARAM: key = key to be found
// POST: Returns pointer to node if found, nullptr if not found.
template <class T, class Y>
NodeT<T,Y>* RedBlackTree<T,Y>::findNode(T key) const
{
	NodeT<T,Y>* current = root;
    // loops until found or nullptr reached
	while (current != nullptr) 
    {
		if (key == current->key) 
        {
			return current;
		}
		if (key < current->key) 
        {
			current = current->left;
		}
		else { // key > current->data
			current = current->right;
		}
	}
    // if does not find value will go down to and return nullptr
	return current;
}

// Searches tree for sucessor node and it's parent
// PRE:
// PARAM: target node to be removed by sucessor method, target node parent pointer reference
// POST: returns pointer to sucessor node and sets reference to it's parent
template <class T, class Y>
NodeT<T,Y>* RedBlackTree<T,Y>::getSuccessor(NodeT<T,Y>* target, NodeT<T,Y>* & sParent) const
{
    // successor is the left most node of the target's right sub-tree
	NodeT<T,Y>* current = target->right;
    sParent = target;

	while (current->left != nullptr) 
    {
        sParent = current;
        current = current->left;
	}

	return current;
}

// Searches tree for predecessor node and it's parent
// PRE:
// PARAM: target node to be removed by predecessor method, target node parent pointer reference
// POST: returns pointer to predecessor node and sets reference to it's parent
template <class T, class Y>
NodeT<T,Y>* RedBlackTree<T,Y>::getPredecessor(NodeT<T,Y>* target, NodeT<T,Y>* & sParent) const
{
    // successor is the left most node of the target's right sub-tree
	NodeT<T,Y>* current = target->left;
    sParent = target;

	while (current->right != nullptr) 
    {
        sParent = current;
        current = current->right;
	}

	return current;
}

// PRE: binary search tree removal method (didn't use in remove but leaving in for referrence.)
// PARAM: key of node you want to remove
// POST: removes node but may leave tree unbalanced
template <class T, class Y>
bool RedBlackTree<T,Y>::bstRemove(T key)
{
    // Find node to remove
    NodeT<T,Y>* parent = nullptr;
    NodeT<T,Y>* target = findNode(key, parent);

    // Target value is not in the tree
    if (target == nullptr)
    {
        return false;
    }

    // target has no children
    if (target->left == nullptr && target->right == nullptr)
    {
        if(parent != nullptr) // i.e target is not the root
        {
            if (target == parent->left) // if left child
            {
                parent->left = nullptr;
            }
            else   // if right child
            {
                parent->right = nullptr;
            }
        }
        else // if target is the root
        {
            root = nullptr;
        }

        delete target;

    }

    // target has one child
    else if(target->left == nullptr || target->right == nullptr)
    {
        // identify child
        NodeT<T,Y>* child = target->left;
        if (child == nullptr)
        {
            child = target->right;
        }

        // attach child to parent, or make child new root
        if(parent != nullptr) // i.e target is not the root
        {
            if(target == parent->left)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }

        }
        else    // target is the root
        {
            root = child;
        }
        delete target;
    }

    // target has two children
    else
    {
        // find successor
        NodeT<T,Y>* successorParent = nullptr;
        NodeT<T,Y>* successor = getSuccessor(target, successorParent);

        // attach target's L child to the sucessor
        successor->left = target->left;

        // attach the successor to target's parent, or amke successor new root
        if(parent != nullptr)
        {
            if(target == parent->left)
            {
                parent->left = successor;
            }
            else
            {
                parent->right = successor;
            }
        }
        else    // i.e target is root
        {
            root = successor;
        }
        // handle case where successor is NOT target's R child
        if(successor != target->right)
        {
            successorParent->left = successor->right;
            successor->right = target->right;
        }
        delete target;
    }
    
    return true;

}

// PRE: binary search tree removal method (didn't end up using in rb Remove but leaving in for referrence.)
// PARAM: pointer to node you want to remove
// POST: removes node but may leave tree unbalanced
template <class T, class Y>
bool RedBlackTree<T,Y>::bstRemove(NodeT<T,Y>* nd)
{
    // Find node to remove
    NodeT<T,Y>* parent = nd->parent;
    NodeT<T,Y>* target = nd;

    // Target value is not in the tree
    if (target == nullptr)
    {
        return false;
    }

    // target has no children
    if (target->left == nullptr && target->right == nullptr)
    {
        if(parent != nullptr) // i.e target is not the root
        {
            if (target == parent->left) // if left child
            {
                parent->left = nullptr;
            }
            else   // if right child
            {
                parent->right = nullptr;
            }
        }
        else // if target is the root
        {
            root = nullptr;
        }

        delete target;

    }

    // target has one child
    else if(target->left == nullptr || target->right == nullptr)
    {
        // identify child
        NodeT<T,Y>* child = target->left;
        if (child == nullptr)
        {
            child = target->right;
        }

        // attach child to parent, or make child new root
        if(parent != nullptr) // i.e target is not the root
        {
            if(target == parent->left)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }

        }
        else    // target is the root
        {
            root = child;
        }
        delete target;
    }

    // target has two children
    else
    {
        // find successor
        NodeT<T,Y>* successorParent = nullptr;
        NodeT<T,Y>* successor = getSuccessor(target, successorParent);

        // attach target's L child to the sucessor
        successor->left = target->left;

        // attach the successor to target's parent, or amke successor new root
        if(parent != nullptr)
        {
            if(target == parent->left)
            {
                parent->left = successor;
            }
            else
            {
                parent->right = successor;
            }
        }
        else    // i.e target is root
        {
            root = successor;
        }
        // handle case where successor is NOT target's R child
        if(successor != target->right)
        {
            successorParent->left = successor->right;
            successor->right = target->right;
        }
        delete target;
    }
    
    return true;

}

// PRE: Fixes unbalanced tree after removal of node
// PARAM: pointer to element that was removed, its parent and wheter it was a right or left child
// POST: balanced red black tree
template <class T, class Y>
void RedBlackTree<T,Y>::rbFix(NodeT<T,Y>* x, NodeT<T,Y>* parent, bool rightChild)
{
    NodeT<T,Y>* y;

    
    while (x != root && isBlack(x) == true) 
    {
      if (rightChild == false) // x is a left child
      {

        // x's sibling  
        y = parent->right;

        if (isBlack(y) == false) // if y is a red node
        {
          y->isBlack = true;
          parent->isBlack = false;
          leftRotate(parent);
          y = parent->right;
        }

        if (isBlack(y->left) == true && isBlack(y->right) == true) 
        {
          y->isBlack = false;
          x = parent;
        } 
        else // x's sibling has one red child
        {
          if (isBlack(y->right) == true) 
          {
            y->left->isBlack = true;
            y->isBlack = false;
            rightRotate(y);
            y = parent->right;
          }

          y->isBlack = parent->isBlack;
          parent->isBlack = true;
          y->right->isBlack = true;
          leftRotate(parent);
          x = root;
        }
      } 
      else // if x is a right child
      {
        y = parent->left;
        if (isBlack(y) == false) 
        {
          y->isBlack = true;
          parent->isBlack = false;
          rightRotate(parent);
          y = parent->left;
        }

        if (isBlack(y->right) == true && isBlack(y->left) == true) 
        {
          y->isBlack = false;
          x = parent;
        } 
        else 
        {
          if (isBlack(y->left) == true) 
          {
            y->right->isBlack = true;
            y->isBlack = false;
            leftRotate(y);
            y = parent->left;
          }

          y->isBlack = parent->isBlack;
          parent->isBlack = true;
          y->left->isBlack = true;
          rightRotate(parent);
          x = root;
        }
      }
    }
    x->isBlack = true;   
}

// Searches tree for target
// PRE:
// PARAM: targetKey = key to be found
// POST: Returns true if targetKey is in tree
template <class T, class Y>
bool RedBlackTree<T,Y>::search(T key) const
{
	NodeT<T,Y>* p = getRoot();
	while (p != nullptr) {
		if (key == p->key) {
			return true;
		}
		else if (key < p->key) {
			p = p->left;
		}
		else {
			p = p->right;
		}
	}
	return false;
}

// Searches tree for keys from value to value
// PRE:
// PARAM: value to search from and to 
// POST: Returns vector of keys from of everything in between and including given key parameters
template <class T, class Y>
std::vector<Y> RedBlackTree<T,Y>::search(T fromKey, T toKey) const
{

    std::vector<Y> stack;

    searchInOrderTraversalCall(getRoot(), stack, fromKey, toKey);

    return stack;

}

// Variations of in order traversal algoritm helper functions
template <class T, class Y>
void RedBlackTree<T,Y>::searchInOrderTraversalCall(NodeT<T,Y>* node, std::vector<Y> &vec, T fromKey, T toKey) const
{
   
    if (node != nullptr) 
    {
    
        searchInOrderTraversalCall(node->left, vec, fromKey, toKey);

        if(node->key >= fromKey && node->key <= toKey)
        {
            vec.push_back(node->value);
        } 

        searchInOrderTraversalCall(node->right, vec, fromKey, toKey);
    }
}

template <class T, class Y>
std::vector<Y> RedBlackTree<T,Y>::values() const
{

    std::vector<Y> stack;

    valuesInOrderTraversalCall(getRoot(), stack);

    return stack;

}

template <class T, class Y>
void RedBlackTree<T,Y>::valuesInOrderTraversalCall(NodeT<T,Y>* node, std::vector<Y> &vec) const
{
   
    if (node != nullptr) 
    {
    
        valuesInOrderTraversalCall(node->left, vec);

        vec.push_back(node->value);

        valuesInOrderTraversalCall(node->right, vec);
    }
}

template <class T, class Y>
std::vector<T> RedBlackTree<T,Y>::keys() const
{

    std::vector<T> stack;

    keysInOrderTraversalCall(getRoot(), stack);

    return stack;

}

template <class T, class Y>
void RedBlackTree<T,Y>::keysInOrderTraversalCall(NodeT<T,Y>* node, std::vector<T> &vec) const
{
   
    if (node != nullptr) 
    {
    
        keysInOrderTraversalCall(node->left, vec);

        vec.push_back(node->key);

        keysInOrderTraversalCall(node->right, vec);
    }
}

template <class T, class Y>
int RedBlackTree<T,Y>::size() const
{
    return tree_size;
}