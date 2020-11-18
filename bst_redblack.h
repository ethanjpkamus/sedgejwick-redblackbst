#include <iostream>
#include <fstream>
#include <cassert>
#include "queue.h"
#include "utils.h"

//this is a left-leaning red-black tree
template <typename Key, typename Value>
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

			Node(Key key_, Value val_, int size_)
	    	: key(key_), val(val_), size(size_), left(nullptr), right(nullptr) {
	    	}

		    friend std::ostream& operator<<(std::ostream& os, const Node& no) {
		      return os << no.left << " <-- "
		                << "(" << no.key << "," << no.val << " (" << &no << ")) --> "
		                << no.right << "\n";
		    }
		};

		Node* root;

		//Node helper functions
		bool is_red(Node* n) 
		{
			return (n == nullptr ? false : n == nullptr);
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

		bool is_empty()
		{
			return root == nullptr;
		}

	/***********************
	 * standard BST search
	 ***********************/
	public: 
		Value get(Key k)
		{
			if(k == Key())
			{
				throw new std::invalid_argument("argument to contains() is null");
			}
			return get(root,k);
		}

	private:
		Value get(Node* x, Key k)
		{
			while(x != nullptr)
			{
				//bool less = (k < x->key ? true : false);
				const fwd_comparator<Key> comp;
				int cmp = compare(k, x->key, &comp);
				if(cmp < 0){
					x = x->left;
				} else if (cmp > 0) {
					x = x->right;
				} else {
					return x->val;
				}

			}
			//return null;
		}

	public:
		bool contains(Key k)
		{
			return get(k) != nullptr;
		}

	/***************************
	 * red-black tree insertion
	 ***************************/

	public:
		void put(Key k, Value v)
		{
			if(k == Key())
			{
				throw new std::invalid_argument("first arguement to put() is null");
			}
			if(v == Value())
			{
				delete_(k);
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

			//TODO: change to compare(T& v, T& w, const comparator<T>& comp) from utils.h
			//bool less = (k < h->key ? true : false);
			const fwd_comparator<Key> comp;
			int cmp = compare(k, h->key, &comp);
			if(cmp < 0){
				h->left = put(h->right, k, v);
			} else if (cmp > 0){
				h->right = put(h->right, k, v);
			} else {
				h->val = v;
			}

			if(is_red(h->right) && !is_red(h->left))
			{
				h = rotate_left(h);
			}
			if(is_red(h->left) && !is_red(h->left->left))
			{
				h = rotate_left(h);
			}
			//check if this is in the correct part of the code
			if(is_red(h->left) && !is_red(h->right))
			{
				flip_colors(h);
			}
			h->size = size(h->left) + size(h->right) + 1;

			return h;
		}

	/**************************
	 * Red-black tree deletion
	 **************************/
	public:
		void delete_min()
		{
			if(is_empty())
			{
				throw new std::logic_error("BST underflow");
			}

			//if both children of root are black, set root to end
			if(!is_red(root->left) && !is_red(root->right))
			{
				root->color = RED;
			}

			root = delete_min(root);
			if(!is_empty())
			{
				root->color = BLACK;
			}
		}

	private:
		Node* delete_min(Node* h)
		{
			if(h->left == nullptr)
			{
				return nullptr; //&Node()
			}

			if(!is_red(h->left) && !is_red(h->left->left))
			{
				h = moveedLeft(h);
			}

			h->left = delete_min(h->left);
			return balance(h);
		}

	/**********************************************************************
	 * Removes the largest key and associated value from the symbol table->
	 **********************************************************************/
	public:
		void delete_max()
		{
			if(is_empty())
			{
				throw new std::logic_error("BST underflow");
			}

			//if both children of root are black, set root to end
			if(!is_red(root->left) && !is_red(root->right))
			{
				root->color = RED;
			}

			root = delete_min(root);
			if(!is_empty())
			{
				root->color = BLACK;
			}
		}

	private:
		Node* delete_max(Node* h)
		{
			if (is_red(h->left))
        	h = rotate_right(h);

        	if (h->right == nullptr)
        	{
        		return nullptr; //&Node()
        	}
            
			if (!is_red(h->right) && !is_red(h->right->left))
            {
            	h = move_red_right(h);
            }

        	h->right = delete_max(h->right);

        	return balance(h);
		}

	/****************************************************************************
	 * Removes the specified key and its associated value from this symbol table     
     * (if the key is in this symbol table)
	 ****************************************************************************/

	public:
		void delete_(Key k) 
		{ 
	        if (k == Key())
			{
				throw new std::invalid_argument("argument to delete_() is null");
			}

	        if (!contains(k))
        	{
        		return;
        	}

	        // if both children of root are black, set root to red
	        if (!is_red(root->left) && !is_red(root->right))
	        {
	            root->color = RED;
	        }

	        root = delete_(root, k);
	        if (!is_empty()) root->color = BLACK;
        // assert check();
    	}

    // delete the key-value pair with the given key rooted at h
    private:
    	Node* delete_(Node* h, Key k) { 
	        // assert get(h, key) != null;
	        //replaces "(key->compareTo(h->key)"
	    	int temp = (k < h->key ? -1 : 1);
	    	if (k == h->key)
	    	{
	    		temp = 0;
	    	}

	        if (temp < 0)  {
	            if (!is_red(h->left) && !is_red(h->left->left))
	                h = move_red_left(h);
	            h->left = delete_(h->left, k);
	        } else {
	            if (is_red(h->left))
	                h = rotate_right(h);
	            if (temp == 0 && (h->right == nullptr))
	                return nullptr; //&Node()
	            if (!is_red(h->right) && !is_red(h->right->left))
	                h = move_red_right(h);
	            if (temp == 0) {
	                Node x = min(h->right);
	                h->key = x->key;
	                h->val = x->val;
	                // h->val = get(h->right, min(h->right)->key);
	                // h->key = min(h->right)->key;
	                h->right = delete_min(h->right);
	            }
	            else h->right = delete_(h->right, k);
	        }
	        return balance(h);
	    }

    /***********************
	 * Red-black tree helper functions
	 ***********************/

    // make a left-leaning link lean to the right
    private:
    	Node* rotate_right(Node* h) {
	        // assert (h != null) && is_red(h->left);
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
    	Node* rotate_left(Node* h) {
	        // assert (h != null) && is_red(h->right);
	        Node x = h->right;
	        h->right = x->left;
	        x->left = h;
	        x->color = x->left->color;
	        x->left->color = RED;
	        x->size = h->size;
	        h->size = size(h->left) + size(h->right) + 1;
	        return x;
    	}

    // flip the colors of a Node and its two children
    private:
    	void flip_colors(Node* h) {
        
	        h->color = !h->color;
	        h->left->color = !h->left->color;
	        h->right->color = !h->right->color;
	    }

	private:
		Node* move_red_left(Node* h) {
	        // assert (h != null);
	        // assert is_red(h) && !is_red(h.left) && !is_red(h.left.left);

	        flip_colors(h);
	        if (is_red(h->right->left)) { 
	            h->right = rotate_right(h->right);
	            h = rotate_left(h);
	            flip_colors(h);
	        }
	        return h;
	    }

    // Assuming that h is red and both h.right and h.right.left
    // are black, make h.right or one of its children red.
    private:
    	Node* move_red_right(Node* h) {
	        // assert (h != null);
	        // assert is_red(h) && !is_red(h.right) && !is_red(h.right.left);
	        flip_colors(h);
	        if (is_red(h->left->left)) { 
	            h = rotate_right(h);
	            flip_colors(h);
	        }
	        return h;
	    }

    // restore red-black tree invariant
	private:
	    Node* balance(Node* h) {
	        // assert (h != null);

	        if (is_red(h->right))
        	{
        		h = rotate_left(h);
        	}
	        if (is_red(h->left) && is_red(h->left->left))
	        { 
	        	h = rotate_right(h);
	        }
	        if (is_red(h->left) && is_red(h->right))
	        {
	        	flip_colors(h);
	        }

	        h->size = size(h->left) + size(h->right) + 1;
	        return h;
	    }

	/***********************
	 * Utility Functions
	 ***********************/
    /*
	public:
		int height() {
	        return height(root);
	    }
    private:
	    int height(Node* x) {
	        if (x == nullptr) return -1;
	        return 1 + Math.max(height(x->left), height(x->right));
	    }
	*/	
    /***********************
	 * Ordered Symbol table functions
	 ***********************/
	public:
		Key min() 
		{
			if (is_empty()) 
			{ 
				throw new std::logic_error("calls min() with empty symbol table"); 
			}
			return min(root)->key;
		}
	private:
		Node* min(Node* x) 
		{ 
			return x->left  == nullptr ? x : min(x->left);  
		}

	public:
		Key max() 
		{
			if (is_empty()) 
			{
				throw new std::logic_error("calls max() with empty symbol table"); 
			}
			return max(root)->key;
		}
	private:
		Node* max(Node* x) 
		{
			return x->right == nullptr ? x : max(x->right); 
		}

	public:
		Key floor(Key& key) 
		{
			if (key == Key())
			{
				throw new std::invalid_argument("argument to floor() is null");
			}
			if (is_empty())
			{ 
				throw new std::logic_error("calls floor() with empty symbol table"); 
			}
			Node x = floor(root, key);
			if (x == nullptr)
			{ 
				throw new std::logic_error("argument to floor() is too small"); 
			} else { 
				return x->key; 
			}
		}

	private:
		Node floor(Node* x, Key key) 
		{
			if (x == nullptr) 
			{
				return nullptr; 
			}

			bool key_less = less(key, x->key);
			bool key_more = less(x->key, key);

			if (key_less && key_more) // equal
			{ 
				return x; 
			}  
			if (key_less)
			{ 
				return floor(x->left, key); 
			}

			Node* t = floor(x->right, key);
			if (t != nullptr) 
			{ 
				return t; 
			}
		}

	public:
		Key floor2(Key& key) 
		{
			Key x = floor2(root, key, nullptr);
			if (x == nullptr)
			{
				throw new std::logic_error("argument to floor() is too small"); 
			} else {
				return x;
			}
		}

	private:
		Key floor2(Node* x, Key key, Key best) 
		{
			if (x == nullptr)
			{ 
				return best; 
			}

			int cmp = key.compareTo(x->key);
			if(cmp  < 0) { 
				return floor2(x->left, key, best); 
			} else if (cmp  > 0) { 
				return floor2(x->right, key, x->key); 
			} else { 
				return x->key; 
			}
		}

	public:
		Key ceiling(Key& key) 
		{
			if (key == Key())
			{ 
				throw new std::invalid_argument("argument to ceiling() is null"); 
			}
			if (is_empty())
			{ 
				throw new std::logic_error("calls ceiling() with empty symbol table"); 
			}
			Node x = ceiling(root, key);
			if (x == Key())
			{ 
				throw new std::logic_error("argument to floor() is too large"); 
			} else { 
				return x->key; 
			}
		}

	private:
		Node* ceiling(Node* x, Key key) 
		{
			if (x == Key())
			{ 
				return nullptr; 
			}

			bool key_less = less(key, x->key);
			bool key_more = less(x->key, key);

			if (key_less && key_more) //equal
			{ 
				return x; 
			}
			if (key_less) {
			  Node* t = ceiling(x->left, key);
			  return t != nullptr ? t : x;
			}
			return ceiling(x->right, key);
		}

	public:
		Key select(int rank) 
		{
			if (rank < 0 || rank >= size()) 
			{
			  std::cerr << "argument to select() is invalid: " << rank << "\n";
			  throw new std::invalid_argument("invalid select");
			}
			return select(root, rank);
		}
	private:
		Key select(Node* x, int rank) 
		{
			if (x == nullptr)
			{ 
				return nullptr; 
			}

			int leftSize = size(x->left);
			if (leftSize > rank) { 
				return select(x->left,  rank); 
			} else if (leftSize < rank) { 
				return select(x->right, rank - leftSize - 1); 
			} else { 
				return x->key; 
			}
		}

	public:
		int rank(Key& key) 
		{
			if (key == Key()) 
			{ 
				throw new std::invalid_argument("argument to rank() is null"); 
			}
			return rank(key, root);
		}

	private:
		int rank(Key key, Node* x) 
		{
			if (x == nullptr) 
			{ 
				return 0; 
			}

			if (less(key, x->key)) { 
				return rank(key, x->left); 
			} else if (less(x->key, key)) { 
				return 1 + size(x->left) + rank(key, x->right); 
			} else { 
				return size(x->left); 
			}
		}

	public:
		void keys(Node* x, queue_<Key>& q, Key low, Key high) 
		{
			if (x == nullptr) 
			{ 
				return; 
			}

			bool low_le  = less(low, x->key)  || low  == x->key;
			bool high_ge = less(x->key, high) || high == x->key;

			if (low_le) // le == less than or equal to (a la Python)
			{ 
				keys(x->left, q, low, high); 
			}   
			if (low_le && high_ge) // ge == greater than or equal to
			{ 
				q.enqueue(x->key); 
			}             
			if (high_ge)
			{ 
				keys(x->right, q, low, high); 
			}
		}

		array_queue<Key> keys() 
		{
			if (is_empty())
			{ 
				return array_queue<Key>(); 
			}
			Key min_key = min(), max_key = max();
			return keys(min_key, max_key);
		}

		array_queue<Key> keys(Key& low, Key& high) 
		{
			if (low == Key())  
			{ 
				throw new std::invalid_argument("first argument to keys() is null"); 
			}
			if (high == Key()) 
			{ 
				throw new std::invalid_argument("second argument to keys() is null"); 
			}

			array_queue<Key> q;
			keys(root, q, low, high);
			return q;
		}

		int size(Key& low, Key& high) 
		{
			if (low == Key())
			{ 
				throw new std::invalid_argument("first argument to size() is null"); 
			}
			if (high == Key()) 
			{ 
				throw new std::invalid_argument("second argument to size() is null"); 
			}

			if (low.compareTo(high) > 0) 
			{ 
				return 0; 
			}
			if (contains(high))
			{ 
				return rank(high) - rank(low) + 1; 
			} else { 
				return rank(high) - rank(low); 
			}
		}

	public:
		int height() 
		{ 
			return height(root); 
		}

	private:
		int height(Node* x) 
		{
			if (x == nullptr) 
			{
				return -1; 
			}
			return 1 + std::max(height(x->left), height(x->right));
		}
	  
	public:
		void print_inorder() 
		{
			std::cout << "========================================================================= printing inorder...\n";
			print_inorder(root);
			std::cout << "========================================================================= end printing inorder...\n\n";
		}

	private:
		void print_inorder(Node* x) {
			if (x == nullptr) 
			{ 
				return; 
			}
			print_inorder(x->left);
			std::cout << *x;
			print_inorder(x->right);
		}

	private:
		//-------- bst validity checks --------------------------------------------------
		bool check() 
		{
			if (!is_bst())
			{ 
				std::cerr << "Not in symmetric order\n";         
				return false;  
			}
			if (!is_size_consistent())
			{ 
				std::cerr << "Subtree counts not consistent\n";  
				return false;  
			}
			if (!is_rank_consistent())
			{ 
				std::cerr << "Ranks not consistent\n";           
				return false;  
			}
			return true;
		}

		bool is_bst() 
		{
			static Key default_key = Key();
			//    print_inorder();

			return is_bst(root, default_key, default_key);
		}

		bool is_bst(Node* x, Key& min, Key& max) {
			if (x == nullptr) 
			{ 
				return true; 
			}
			Key default_key = Key();

			if (min != default_key && less(x->key, min) <= 0) 
			{ 
				return false; 
			}
			if (max != default_key && less(max, x->key))   
			{ 
				return false; 
			}
			bool left_bst = is_bst(x->left, min, x->key);
			bool right_bst = is_bst(x->left, min, x->key);
			return (left_bst && right_bst);
		}

		bool is_size_consistent() 
		{ 
			return is_size_consistent(root); 
		}

		bool is_size_consistent(Node* x) 
		{
			if (x == nullptr) 
			{ 
				return true; 
			}
			if (x->size != size(x->left) + size(x->right) + 1) 
			{ 
				return false; 
			}
			return ( is_size_consistent(x->left) && is_size_consistent(x->right) );
		}

		bool is_rank_consistent() 
		{
			for (int i = 0; i < size(); i++) 
			{
				Key key_selected = select(i);
				if (i != rank(key_selected)) 
					{ 
						return false; 
					}
			}
			for (Key& key : keys()) 
			{
				Key key_at_rank = select(rank(key));
				if (less(key, key_at_rank) || less(key_at_rank, key)) // if !equal
				{  
					return false;
				}
			}
			return true;
		}

		array_queue<Key> level_order() 
		{
			array_queue<Key> keys;
			array_queue<Node*> q;

			q.enqueue(root);
			while (!q.empty()) 
			{
				Node* x = q.dequeue();
				if (x == nullptr) 
				{ 
					continue; 
				}

				keys.enqueue(x->key);
				q.enqueue(x->left);
				q.enqueue(x->right);
			}
			return keys;
		}
	
	public:
		//change to run_tests(){...}
		static void run_tests()
		{
			test_bst("tinyST.txt");
			test_bst("gettysburg_ST.txt");
		}
		static void test_bst(const std::string& filename) {
			char buf[BUFSIZ];
			bst<std::string, int> st;

			std::ifstream ifs(filename);
			if (!ifs.is_open()) 
			{
				std::cerr << "Could not open file: '" << filename << "'\n";  exit(2);
			}

			std::cout << "Building symbol table for file: '" << filename << "'. \n";
			std::cout << "(punctuation automatically stripped)... \n";
			int i = 0;
			std::string s;
			while (ifs >> s)
			{
				strcpy(buf, s.c_str());
				strconvert(buf, tolower);
				strstrip(buf);
				std::string key = std::string(buf);
				if (key != "") 
				{ 
					st.put(key, i++); 
				}
				//      st.print_inorder();
			}
			std::cout << "\n";

			std::cout << "\nin level order (root to leaves)...\n";
			array_queue<Key> keys = st.level_order();
			for (std::string& key : keys) {
			  std::cout << std::setw(12) << key << "  " << std::setw(2) << st.get(key) << "\n";
			}

			std::cout << "\nin alphabetical order...\n";
			keys = st.keys();
			for (std::string& key : keys) {
			  std::cout << std::setw(14) << key << "  " << std::setw(2) << st.get(key) << "\n";
			}
		}
}; //end of class def