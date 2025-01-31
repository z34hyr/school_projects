
#pragma once

template <typename ValType>
	struct bst_node
{
	typedef	bst_node*	bst_ptr;
	typedef const bst_node* bst_const_ptr;
	bst_ptr				left;
	bst_ptr				right;
	bst_ptr				parent;
	ValType				value;

	bst_node() : left(NULL), right(NULL), parent(NULL) {};
	bst_node(ValType value, bst_ptr parent) : left(NULL), right(NULL), parent(parent), value(value) {};

	bst_ptr	min(bst_ptr current)
	{
		while (current && current->right != NULL)
			current = current->right;
		return current;
	}
	bst_const_ptr	min(bst_const_ptr current) const
	{
		while (current && current->right != NULL)
			current = current->right;
		return current;
	}
	bst_ptr	max(bst_ptr current)
	{
		while (current && current->left != NULL)
			current = current->left;
		return current;
	}
	bst_const_ptr	max(bst_const_ptr current) const
	{
		while (current && current->left != NULL)
			current = current->left;
		return current;
	}
	bool	is_right_child()
	{
		if (this->parent && this->parent->right == this)
			return true;
		return false;
	}

};
template <typename Value>
	struct bst_const_iterator;

template <typename Value>
	struct bst_iterator
{
	public:
		typedef bst_node<Value>* node_ptr;
		typedef Value&	reference;
		typedef Value*	pointer;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef bst_const_iterator<Value>	bst_const_iterator;
	private:
		node_ptr	m_node;

	public:
		bst_iterator() { m_node = NULL; };
		bst_iterator(node_ptr ptr) : m_node(ptr) {};
		operator bst_const_iterator() { return bst_const_iterator(m_node); };
		~bst_iterator() {};

		node_ptr get_node() { return this->m_node; };

		reference operator*() const { return m_node->value; };
		pointer operator->() const { return &m_node->value; };
		bst_iterator& operator= (bst_iterator const & obj) { this->m_node = obj.m_node; return *this; }; //assignatiom overloading
		bool operator==(bst_iterator & iter) { return this->m_node == iter.m_node; };
		bool operator!=(bst_iterator & iter) { return !this->operator==(iter); };
		bst_iterator&	operator++()
		{
			if (m_node->left)
				this->m_node = m_node->min(m_node->left);
			else
			{
				while (m_node->parent && m_node == m_node->parent->left)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_iterator	operator++(int)
		{
			bst_iterator temp = *this;
			*this = this->operator++();
			return temp;
		}
		bst_iterator&	operator--()
		{
			if (m_node->right)
				m_node = m_node->max(m_node->right);
			else
			{
				while (m_node->parent && m_node == m_node->parent->right)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_iterator	operator--(int)
		{
			bst_iterator temp = *this;
			*this = this->operator--();
			return temp;
		}
};

template <typename Value>
	struct bst_const_iterator
{
	public:
		typedef const bst_node<Value>* node_const_ptr;
		typedef const Value&	reference;
		typedef const Value*	pointer;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef bst_iterator<Value>	bst_iterator;
	private:
		node_const_ptr	m_node;

	public:
		bst_const_iterator() { m_node = NULL; };
		bst_const_iterator(node_const_ptr ptr) : m_node(ptr) {};
		bst_const_iterator(bst_iterator& val) : m_node(val.get_node()) {};

		node_const_ptr get_node() const { return this->m_node; };

		reference operator*() const { return m_node->value; };
		pointer operator->() const { return &m_node->value; };
		bst_const_iterator& operator= (bst_const_iterator const & obj) { this->m_node = obj.m_node; return *this; }; //assignatiom overloading
		bool operator==(bst_const_iterator & iter) { return this->m_node == iter.m_node; };
		bool operator!=(bst_const_iterator & iter) { return !this->operator==(iter); };
		bst_const_iterator&	operator++()
		{
			if (m_node->left)
				this->m_node = m_node->min(m_node->left);
			else
			{
				while (m_node->parent && m_node == m_node->parent->left)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_const_iterator&	operator++(int)
		{
			bst_const_iterator temp = *this;
			this->operator++();
			return temp;
		}
		bst_const_iterator&	operator--()
		{
			if (m_node->right)
				m_node = m_node->max(m_node->right);
			else
			{
				while (m_node->parent && m_node == m_node->parent->right)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_const_iterator&	operator--(int)
		{
			bst_const_iterator temp = *this;
			this->operator--();
			return temp;
		}

};

template <typename Value>
	struct bst_const_reverse_iterator;

template <typename Value>
	struct bst_reverse_iterator
{
	public:
		typedef bst_node<Value>* node_ptr;
		typedef Value&	reference;
		typedef Value*	pointer;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef bst_const_reverse_iterator<Value>	bst_const_reverse_iterator;
		
	private:
		node_ptr	m_node;

	public:
		bst_reverse_iterator() { m_node = NULL; };
		bst_reverse_iterator(node_ptr ptr) : m_node(ptr) {};
		operator bst_const_reverse_iterator() { return bst_const_reverse_iterator(m_node); };
		~bst_reverse_iterator() {};

		node_ptr get_node() { return this->m_node; };

		reference operator*()
		{
			bst_reverse_iterator temp = *this;
			++temp;
			return temp.m_node->value; 
		};
		pointer operator->() { return &(operator*()); };
		bst_reverse_iterator& operator= (bst_reverse_iterator const & obj) { this->m_node = obj.m_node; return *this; }; //assignatiom overloading
		bool operator==(bst_reverse_iterator & iter) { return this->m_node == iter.m_node; };
		bool operator!=(bst_reverse_iterator & iter) { return !this->operator==(iter); };
		bst_reverse_iterator&	operator++()
		{
			if (m_node->right)
				m_node = m_node->max(m_node->right);
			else
			{
				while (m_node->parent && m_node == m_node->parent->right)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_reverse_iterator	operator++(int)
		{
			bst_reverse_iterator temp = *this;
			*this = this->operator++();
			return temp;
		}
		bst_reverse_iterator&	operator--()
		{
			if (m_node->left)
				this->m_node = m_node->min(m_node->left);
			else
			{
				while (m_node->parent && m_node == m_node->parent->left)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_reverse_iterator	operator--(int)
		{
			bst_reverse_iterator temp = *this;
			*this = this->operator--();
			return temp;
		}
};

template <typename Value>
	struct bst_const_reverse_iterator
{
	public:
		typedef bst_node<Value>* node_ptr;
		typedef Value&	reference;
		typedef Value*	pointer;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
	private:
		node_ptr	m_node;

	public:
		bst_const_reverse_iterator() { m_node = NULL; };
		bst_const_reverse_iterator(node_ptr ptr) : m_node(ptr) {};
		~bst_const_reverse_iterator() {};

		node_ptr get_node() const { return this->m_node; };

		reference operator*() const
		{
			bst_const_reverse_iterator temp = *this;
			++temp;
			return temp.m_node->value; 
		};
		pointer operator->() const { return &(operator*()); };
		bst_const_reverse_iterator& operator= (bst_const_reverse_iterator const & obj) { this->m_node = obj.m_node; return *this; }; //assignatiom overloading
		bool operator==(bst_const_reverse_iterator & iter) { return this->m_node == iter.m_node; };
		bool operator!=(bst_const_reverse_iterator & iter) { return !this->operator==(iter); };
		bst_const_reverse_iterator&	operator++()
		{
			if (m_node->right)
				m_node = m_node->max(m_node->right);
			else
			{
				while (m_node->parent && m_node == m_node->parent->right)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_const_reverse_iterator	operator++(int)
		{
			bst_const_reverse_iterator temp = *this;
			*this = this->operator++();
			return temp;
		}
		bst_const_reverse_iterator&	operator--()
		{
			if (m_node->left)
				this->m_node = m_node->min(m_node->left);
			else
			{
				while (m_node->parent && m_node == m_node->parent->left)
					m_node = m_node->parent;
				m_node = m_node->parent;
			}
			return *this;
		}
		bst_const_reverse_iterator	operator--(int)
		{
			bst_const_reverse_iterator temp = *this;
			*this = this->operator--();
			return temp;
		}
};

template <class Key, class T, class Compare >
	class bst
{
	public:
		typedef Key									key_type;
		typedef T									mapped_type;
		typedef std::pair<const key_type, mapped_type>	value_type;
		typedef Compare								key_compare;
		typedef value_type&							reference;
		typedef value_type*							pointer;
		typedef bst_node<value_type>				node;
		typedef bst_iterator<value_type>			iterator;
		typedef bst_const_iterator<value_type>		const_iterator;
		typedef bst_reverse_iterator<value_type>	reverse_iterator;
		typedef bst_const_reverse_iterator<value_type>	const_reverse_iterator;
		typedef size_t								size_type;

	private:
		node*	root_ptr;
		node	end_ptr;
		size_type	_size;
		Compare	compare;

	public:
		~bst() { this->clear(); };
		bst(const key_compare &comp = key_compare()) : compare(comp)
		{
			root_ptr = NULL;
			_size = 0;
		};
		iterator	begin()
		{
			return iterator(root_ptr ? root_ptr->min(root_ptr) : &end_ptr);
		}
		const_iterator	begin() const
		{
			return const_iterator(root_ptr ? root_ptr->min(root_ptr) : &end_ptr);
		}
		iterator	end()
		{
			return iterator(&end_ptr);
		}
		const_iterator	end() const
		{
			return const_iterator(&end_ptr);
		}
		reverse_iterator	rbegin()
		{
			return reverse_iterator(&end_ptr);
		}
		reverse_iterator	rend()
		{
			return reverse_iterator(root_ptr ? (root_ptr->min(root_ptr)) : &end_ptr);
		}
		const_reverse_iterator	rbegin() const
		{
			return reverse_iterator(&end_ptr);
		}
		const_reverse_iterator	rend() const
		{
			return const_reverse_iterator(root_ptr ? (root_ptr->min(root_ptr)) : &end_ptr);
		}
		const key_compare&	key_comp() const
		{
			return compare;
		}
		key_compare&	key_comp()
		{
			return compare;
		}

		iterator	find(key_type find_key)
		{
			iterator	i_b = this->begin();
			iterator	i_e = this->end();
			while (i_b != i_e)
			{
				if ((*i_b).first == find_key)
					return i_b;
				++i_b;
			}
			return i_e;
		}
		iterator	lower_bound(key_type find_key)
		{
			iterator	i_b = this->begin();
			iterator	i_e = this->end();
			while (i_b != i_e)
			{
				if (!compare_keys((*i_b).first, find_key))
					return i_b;
				++i_b;
			}
			return i_e;
		}
		iterator	upper_bound(key_type find_key)
		{
			iterator	i_b = this->begin();
			iterator	i_e = this->end();
			while (i_b != i_e)
			{
				if (compare_keys(find_key, (*i_b).first))
					return i_b;
				++i_b;
			}
			return i_e;
		}
		const_iterator	find(key_type find_key) const
		{
			const_iterator	i_b = this->begin();
			const_iterator	i_e = this->end();
			while (i_b != i_e)
			{
				if (i_b->first == find_key)
					return i_b;
				++i_b;
			}
			return i_e;
		}
		mapped_type& operator[] (const key_type& k)
		{
			iterator i_by_key = find(k);
			iterator i_e = end();
			if (i_by_key == i_e)
			{
				std::pair<const key_type, mapped_type> new_val(k, 0);
				insert(new_val);
			}
			return this->find(k)->second;
		};
		bool	compare_keys(const key_type &a, const key_type &b) const
		{
			return (compare(a, b));
		}
		size_type	get_size() const
		{
			return _size;
		}
		void	clear()
		{
			iterator	i_b = this->begin();
			iterator	i_e = this->end();
			while (i_b != i_e)
			{
				remove((*i_b).first);
				i_b = this->begin();
			}
			root_ptr = NULL;
		}
		size_type	remove(key_type const & rem_key)
		{
			iterator	to_rem = this->find(rem_key);
			iterator	i_e = this->end();
			if (to_rem != i_e)
			{
				node*	rem_node = to_rem.get_node();
				if (rem_node->right && rem_node->left)
				{
					node* temp = rem_node->min(rem_node->left);
					temp->right = rem_node->right;
					rem_node->right->parent = temp;
					rem_node->right = NULL;
				}
				node*	temp_ass = (rem_node->right ? rem_node->right : rem_node->left);
				if (rem_node->parent)
				{
					if (rem_node->is_right_child())
						rem_node->parent->right = temp_ass;
					else
						rem_node->parent->left = temp_ass;
					if (temp_ass)
						temp_ass->parent = rem_node->parent;
				}
				else
				{
					root_ptr = temp_ass;
					if (temp_ass)
						temp_ass->parent = NULL;
				}
				_size -= 1;
				delete rem_node;
				return 1;				
			}
			return 0;
		}
		iterator insert (iterator position, const value_type& val)
		{
			iterator i_e = this->end();
			node* temp = position.get_node();
			if (i_e != position && temp->left == &end_ptr && compare(temp->value.first, val.first))
			{
				node* new_node = new node(val, temp);
				temp->left = new_node;
				new_node->left = &end_ptr;
				_size += 1;
				return iterator(new_node);
			}
			else
			{
				std::pair<iterator,bool> rez = this->insert(val);
				return rez.first;
			}
			return i_e;
		}
		std::pair<iterator,bool>	insert(value_type new_value)
		{
			iterator to_return;
			if (!root_ptr)
			{
				root_ptr = new node(new_value, NULL);
				//root_ptr->value = new_value;
				root_ptr->left = &end_ptr;
				end_ptr.parent = root_ptr;

				to_return = iterator(root_ptr);
			}
			else
			{
				node* temp = root_ptr;
				node* temp_last = temp;

				while (temp && temp != &end_ptr)
				{
					if (new_value.first == temp->value.first)
					{
						// change value
						temp->value.second = new_value.second;

						to_return = iterator(temp);
						return std::pair<iterator,bool>(to_return, false);
					}
					else if (compare_keys(new_value.first, temp->value.first))
						temp = temp->right;

					else
						temp = temp->left;
					if (temp && temp != &end_ptr)
						temp_last = temp;
				}
				node* new_node = new node(new_value, temp_last);
				if (temp_last->left == &end_ptr && compare_keys(temp_last->value.first, new_value.first))// new_value.first > temp_last->value.first)
				{
					temp_last->left = new_node;
					new_node->left = &end_ptr;
					end_ptr.parent = new_node;
				}
				else
					compare_keys(temp_last->value.first, new_value.first) ? temp_last->left = new_node : temp_last->right = new_node;				
				to_return = iterator(new_node);
			}
			_size += 1;
			return std::pair<iterator,bool>(to_return, true);
		};
		// this part is for SWAP function
		inline size_type	set_size(size_type new_size)
		{
			size_type temp_size = _size;
			_size = new_size;
			return temp_size;
		}
		inline node*	get_root()
		{
			return root_ptr;
		}
		inline node*	get_prev()
		{
			return end_ptr.parent;
		}
		inline node*	get_end()
		{
			return &end_ptr;
		}
		inline void	set_root(node* root)
		{
			root_ptr = root;
		}
		inline void set_prev(node* prev)
		{
			end_ptr.parent = prev;
		}
		inline void	swap(bst& x)
		{
			node* temp_root = x.get_root();
			node* temp_prev = x.get_prev();
			node* temp_end = x.get_end();

			x.set_root(root_ptr);
			temp_end->parent = end_ptr.parent;
			if (end_ptr.parent)
				end_ptr.parent->left = temp_end;

			root_ptr = temp_root;
			if (temp_prev)
				temp_prev->left = &end_ptr;
			end_ptr.parent = temp_prev;

			_size = x.set_size(_size);
		}
};
