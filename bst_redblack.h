#include <iostream>
#include <fstream>
#include <cassert>
#include "queue.h"

template <typename Key, typename, Value>
class bst_redblack {
	
	public:
		bst_redblack() : root(nullptr) { }

	private:
		static const bool RED = true;
		static const bool BLACK = false;

		struct Node 
		{
			Key key;
			Value val;
			Node *left, *right;
			bool color;
			int size;

			node(Key key_, Value val_, int size_)
	    	: key(key_), val(val_), size(size_), left(nullptr), right(nullptr) {
	    	}
		}

		Node* root;

		//node helper functions
		bool isRed(Node* n) 
		{
			if(n == nullptr)
			{
				return false;
			}
			return n->color == RED;
		}

		int size(Node* n)
		{
			if(n == nullptr)
			{
				return 0;
			}
			return n->size;
		}

		int size()
		{
			return size(root);
		}

		bool isEmpty()
		{
			return root == nullptr;
		}

	/***********************
	 * standard BST search
	 ***********************/
	public: 
		Value get(Key k)
		{
			if(k == null)
			{
				throw new std::invalid_argument("argument to contains() is null");
			}
			return get(root,key);
		}
	private:
		Value get(Node x, Key k)
		{
			while(x != nullptr)
			{
				bool less = (x->key < k ? true : false);
				if(less){
					x = x->left;
				} else if (!less){
					x = x->right;
				} else {
					return x->val;
				}

			}
			return null;
		}

	public:
		bool contains(Key k)
		{
			return get(k) != null;
		}

	/***************************
	 * red-black tree insertion
	 ***************************/

	public:
		void put(Key k, Value v)
		{
			if(key == null)
			{
				throw new std::invalid_argument("first arguement to put() is null");
			}
			if(val == null)
			{
				delete(k);
				return;
			}

			root = put(root,k,v);
			root->color = BLACK;
		}

	private:
		Node* put(Node* h, Key k, Value v)
		{
			if(h == nullptr)
			{
				return new Node(k, v, RED, 1);
			}

			bool less = (x->key < k ? true : false);
			if(less){
				h->left = put(h->right, k, v);
			} else if (!less){
				h->right = put(h->right, k, v);
			} else {
				h->val = v;
			}

			if(isRed(h->right) && !isRed(h->left))
			{
				h = rotateLeft(h);
			}
			if(isRed(h->left) && !isRed(h->left->left))
			{
				h = rotateLeft(h);
			}
			if(isRed(h->left) && !isRed(h->right))
			{
				flipColors(h);
			}
			h->size = size(h->left) + size(h->right) + 1;

			return h;
		}

	/**************************
	 * Red-black tree deletion
	 **************************/
	public:
		void deleteMin()
		{
			if(isEmpty())
			{
				throw new std::logic_error("BST underflow");
			}

			//if both children of root are black, set root to end
			if(!isRed(root->left) && !isRed(root->right))
			{
				root->color = RED;
			}

			root = deleteMin(root);
			if(!isEmpty())
			{
				root->color = BLACK;
			}
		}

	private:
		Node* deleteMin(Node* h)
		{
			if(h->left == nullptr)
			{
				return null;
			}

			if(!isRed(h->left) && !isRed(h->left->left))
			{
				h = moveedLeft(h);
			}

			h->left = deleteMin(h->left);
			return balance(h);
		}

	/**********************************************************************
	 * Removes the largest key and associated value from the symbol table->
	 **********************************************************************/
	public:
		void deleteMax()
		{
			if(isEmpty())
			{
				throw new std::logic_error("BST underflow");
			}

			//if both children of root are black, set root to end
			if(!isRed(root->left) && !isRed(root->right))
			{
				root->color = RED;
			}

			root = deleteMin(root);
			if(!isEmpty())
			{
				root->color = BLACK;
			}
		}

	private:
		Node* deleteMax(Node* h)
		{
			if (isRed(h->left))
        	h = rotateRight(h);

        	if (h->right == null)
        	{
        		return null;
        	}
            
			if (!isRed(h->right) && !isRed(h->right->left))
            {
            	h = moveRedRight(h);
            }

        	h->right = deleteMax(h->right);

        	return balance(h);
		}

	/***********************
	 * Removes the specified key and its associated value from this symbol table     
     * (if the key is in this symbol table)-> 
	 ***********************/

	public:
		void delete(Key k) 
		{ 
	        if (key == null)
			{
				throw new std::invalid_argument("argument to delete() is null");
			}
	        if (!contains(key))
        	{
        		return;
        	}

	        // if both children of root are black, set root to red
	        if (!isRed(root->left) && !isRed(root->right))
	        {
	            root->color = RED;
	        }

	        root = delete(root, k);
	        if (!isEmpty()) root->color = BLACK;
        // assert check();
    	}

    // delete the key-value pair with the given key rooted at h
    private:
    	Node* delete(Node* h, Key key) { 
        // assert get(h, key) != null;

        if (key->compareTo(h->key) < 0)  {
            if (!isRed(h->left) && !isRed(h->left->left))
                h = moveRedLeft(h);
            h->left = delete(h->left, key);
        }
        else {
            if (isRed(h->left))
                h = rotateRight(h);
            if (key->compareTo(h->key) == 0 && (h->right == null))
                return null;
            if (!isRed(h->right) && !isRed(h->right->left))
                h = moveRedRight(h);
            if (key->compareTo(h->key) == 0) {
                Node x = min(h->right);
                h->key = x->key;
                h->val = x->val;
                // h->val = get(h->right, min(h->right)->key);
                // h->key = min(h->right)->key;
                h->right = deleteMin(h->right);
            }
            else h->right = delete(h->right, key);
        }
        return balance(h);
    }

    /***********************
	 * Red-black tree helper functions
	 ***********************/

    // make a left-leaning link lean to the right
    private:
    	Node* rotateRight(Node* h) {
	        // assert (h != null) && isRed(h->left);
	        Node x = h->left;
	        h->left = x->right;
	        x->right = h;
	        x->color = x->right->color;
	        x->right->color = RED;
	        x->size = h->size;
	        h->size = size(h->left) + size(h->right) + 1;
	        return x;
	    }

    // make a right-leaning link lean to the left
    private:
    	Node* rotateLeft(Nod* h) {
	        // assert (h != null) && isRed(h->right);
	        Node x = h->right;
	        h->right = x->left;
	        x->left = h;
	        x->color = x->left->color;
	        x->left->color = RED;
	        x->size = h->size;
	        h->size = size(h->left) + size(h->right) + 1;
	        return x;
    	}

    // flip the colors of a node and its two children
    private:
    	void flipColors(Node* h) {
        
	        h->color = !h->color;
	        h->left->color = !h->left->color;
	        h->right->color = !h->right->color;
	    }

};