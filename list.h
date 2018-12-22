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
        pointer prev;
        pointer next;
        T data;
		list_node(const T& d, pointer p, pointer n):
			data(d), prev(p), next(n){}
        list_node(){ prev = next = nullptr; }
		list_node(const list_node& t):prev(t.prev),next(t.next),data(t.data){ }
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




    template<typename T, typename Alloc = allocator<list_node<T> > >
    class list{
    protected:
        typedef list_node<T> list_node;
    public:
        typedef list_node* link_type;
        typedef list_iterator<T> iterator;
		typedef list_iterator<const T> const_iterator;
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
				Alloc::destroy(temp.node);
				Alloc::deallocate(temp.node);
			}
			Alloc::deallocate(tail.node);
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
			
		}
		
		void pop_front(){
			iterator p = head++;
			head.node->prev = end();
			tail.node->next = head.node;
			deleteNode(p.node);
		}


		void push_back(const value_type& val);
		void pop_back();

		iterator begin(){ return head; }
		iterator end(){ return tail; }
		// const_iterator begin()const;
		// const_iterator end()const;
		// reverse_iterator rbegin();
		// reverse_iterator rend();

		iterator insert(iterator position, const value_type& val)
		{
			if (position == begin()){
				push_front(val);
				return begin();
			}else if (position == end()){
				iterator ret = position;
				push_back(val);
				return ret;
			}
		}
		void insert(iterator position, size_type n, const value_type& val);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(list& x);
		void clear();
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
			link_type res = Alloc::allocate();
			Alloc::construct(res, list_node(val, nullptr, nullptr));
			return res;
		}
		void deleteNode(link_type p){
			p->prev = p->next = nullptr;
			Alloc::destroy(p);
			Alloc::deallocate(p);
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