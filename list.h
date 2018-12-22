#ifndef __LIST_H__
#define __LIST_H__

#include "alloc.h"
#include <cstddef>  // prtdiff_t, size_t
#include "allocator.h"
#include "iterator.h"

namespace TMPS{

    template<typename T>
    struct list_node{
        typedef list_node* pointer;
        T data;
        pointer prev;
        pointer next;
		list_node(const T& d, pointer p, pointer n):
			data(d), prev(p), next(n){}
        list_node(){ prev = next = nullptr; }
		list_node(const list_node& t):data(t.data),prev(t.prev),next(t.next){ }
    };

    template<typename T>
    struct list_iterator : public iterator<bidirectional_iterator_tag, T>{
        // typedef bidirectional_iterator_tag iterator_category;
        // typedef T value_type;
        // typedef T* pointer;
        // typedef T& reference;
        // typedef ptrdiff_t difference_type;

        typedef list_iterator<T> self;
        typedef list_node<T>* link_type;

        link_type node;

		list_iterator(){}
		list_iterator(link_type x):node(x){}
		list_iterator(const list_iterator& x):node(x.node){}

        T& operator*() const { return (*node).data; }
        T* operator->() const { return &(operator*()); }
        self& operator++() {
            node = (link_type)((*node).next); 
            return *this;
        }
        self operator++(int) {
            self temp = *this;
            ++(*this);
            return temp;
        }
        self& operator--() {
            node = (link_type)((*node).prev); 
            return *this;
        }
    
        self operator--(int) {
            self temp = *this;
            --(*this);
            return temp;
        }
		bool operator==(const self& t)const
		{
			return node == t.node;
		}
		bool operator!=(const self& t)const
		{
			return node != t.node;
		}
    };




    template<typename T, typename Alloc = alloc >
    class list{
    protected:
        typedef list_node<T> list_node;
    public:
        typedef list_node* link_type;
        typedef list_iterator<T> iterator;
		typedef list_iterator<const T> const_iterator;

		typedef allocator<list_node, Alloc> list_node_allocator;
		// typedef reverse_iterator_t<iterator> reverse_iterator;
    protected:
        link_type node;
	private:
		iterator head;
		iterator tail;
    public:
		typedef T value_type;
		typedef T& reference;
		typedef size_t size_type;
	public:
		list() {
            node = newNode();

			node->next = node;
			node->prev = node;

		    head.node = node;
			tail.node = node;
        }

		explicit list(size_type n, const value_type& val = value_type()) {

        }

		template <class InputIterator>
		list(InputIterator first, InputIterator last);

		list(const list& l) {
			head.node = newNode();
			tail.node = head.node;
			for( link_type node = l.head.node; node != l->tail.node; node = node->next){
				push_back(node->data);
			}
        }

		list& operator = (const list& l);
		~list(){
			while(head != tail) {
				iterator temp = head++;
				deleteNode(temp.node);
			}
			deleteNode(tail.node);
		}

		bool empty()const{ return head == tail; }
		size_type size()const
		{
			size_type re = 0;
			for( iterator it = begin(); it != end(); it++){
				re++;
			}
			return re;
		}
		reference front(){ return (head.node->data); }
		reference back(){ return (tail.node->prev->data); }

		void push_front(const value_type& val){
			link_type t = newNode();
			
			t->data = val;
			t->next = begin().node;
			t->prev = end().node;
			
			begin().node->prev = t;
			end().node->next = t;
			
			head.node = t;
		}
		
		void pop_front(){
			iterator p = head++;
			head.node->prev = end().node;
			tail.node->next = head.node;
			deleteNode(p.node);
		}


		void push_back(const value_type& val){
			if(begin() == end()){
				push_front(val);
			}else{
				link_type t = newNode();

				t->data = val;
				t->next = end().node;
				t->prev = end().node->prev;
				
				end().node->prev->next = t;
				end().node->prev = t;
			}
		}
		void pop_back(){
			iterator p = tail--;
			deleteNode(p.node);
		}

		iterator begin(){ return head; }
		iterator end(){ return tail; }
		// const_iterator begin()const;
		// const_iterator end()const;
		// reverse_iterator rbegin();
		// reverse_iterator rend();

		iterator insert(iterator position, const value_type& val)
		{
			iterator re;
			if (position == begin()){
				push_front(val);
				re = begin();
			}else if (position == end()){
				iterator ret = position;
				push_back(val);
				re = ret;
			}else{
				link_type t = newNode();
				t->data = val;
				t->next = position.node;
				t->prev = position.node->prev;
				t->prev->next = t;
				t->next->prev = t;
				re = --position;
			}

			return re;
		}
		void insert(iterator position, size_type n, const value_type& val)
		{
			for( size_type i = 0; i < n; i++){
				insert(position,val);
			}
		}
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(list& x);
		void clear(){
			while(head != tail){
				iterator p = head++;
				deleteNode(p.node);
			}
		}
		void splice(iterator position, list& x);
		void splice(iterator position, list& x, iterator i);
		void splice(iterator position, list& x, iterator first, iterator last);
		void remove(const value_type& val);
		template <class Predicate>
		void remove_if(Predicate pred);
		void unique();
		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(list& x);
		template <class Compare>
		void merge(list& x, Compare comp);
		void sort();
		template <class Compare>
		void sort(Compare comp);
		void reverse();
	private:
		void ctorAux(size_type n, const value_type& val, std::true_type);
		template <class InputIterator>
		void ctorAux(InputIterator first, InputIterator last, std::false_type);

		link_type newNode(const T& val = T()){
			link_type res = list_node_allocator::allocate();
			list_node_allocator::construct(res, list_node(val, nullptr, nullptr));
			return res;
		}

		void deleteNode(link_type p){
			p->prev = p->next = nullptr;
			list_node_allocator::destroy(p);
			list_node_allocator::deallocate(p);
		}
		void insert_aux(iterator position, size_type n, const T& val, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		const_iterator changeIteratorToConstIterator(iterator& it)const;
	public:
		friend void swap(list<T>& x, list<T>& y);
		friend bool operator== (const list<T>& lhs, const list<T>& rhs);
		friend bool operator!= (const list<T>& lhs, const list<T>& rhs);
    // public:
        // list(const Allocator& alloc){}
        // explicit list( size_type count, const T& value = T(),const Allocator& alloc = Allocator()){}
        // template< class InputIt >
        // list( InputIt first, InputIt last, const Allocator& alloc = Allocator() ){}
        // list( const list& other, const Allocator& alloc ){}
        // list( list&& other, const Allocator& alloc ){}
        // list( std::initializer_list<T> init, const Allocator& alloc = Allocator() ){}
    };














}




#endif /* __LIST_H__ */