#ifndef __LIST_H__
#define __LIST_H__

#include "alloc.h"
#include <cstddef>  // prtdiff_t, size_t
#include "allocator.h"
#include "iterator.h"
#include "algorithm.h"
#include "reverseIterator.h"

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
		list_node(const list_node& t):data(t.data),prev(t.prev),next(t.next){}
    };

    template<typename T>
    struct list_iterator : public iterator<bidirectional_iterator_tag, T>{
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ptrdiff_t difference_type;

        typedef list_iterator<T> self;
        typedef list_node<T>* link_type;

        link_type node;

		list_iterator(){}
		list_iterator(link_type x):node(x){}
		list_iterator(const list_iterator& x):node(x.node){}

        reference operator*() const { return (*node).data; }
        pointer operator->() const { return &(operator*()); }
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

    template<typename T>
    struct const_list_iterator : public iterator<bidirectional_iterator_tag, T>{
        typedef bidirectional_iterator_tag iterator_category;
        typedef const T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef ptrdiff_t difference_type;

        typedef const_list_iterator<T> self;
        typedef list_node<T>* link_type;

        link_type node;

		const_list_iterator(){}
		const_list_iterator(link_type x):node(x){}
		const_list_iterator(const const_list_iterator& x):node(x.node){}
		const_list_iterator(const list_iterator<T>& x):node(x.node){}


        reference operator*() const { return (*node).data; }
        pointer operator->() const { return &(operator*()); }
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
		// typedef list_node<const T> const_list_node;
        typedef list_node<T> list_node;
    public:
        typedef list_node* link_type;
        typedef list_iterator<T> iterator;
		typedef const_list_iterator<T> const_iterator;
		typedef reverse_iterator<iterator> reverse_iterator;

		typedef allocator<list_node, Alloc> list_node_allocator;
		typedef allocator<T, Alloc> data_allocator;
		
    protected:
        link_type node;
	private:
		iterator head;
		iterator tail;
    public:
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef size_t size_type;
	public:

		bool empty()const{ return head == tail; }
		size_type size()const{
			size_type re = 0;
			for( iterator it = begin(); it != end(); it++){
				re++;
			}
			return re;
		}

		reference front(){ return (head.node->data); }
		reference back(){ return (tail.node->prev->data); }

		void push_front(const value_type& val){
			link_type t = newNode(val);
			
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
				link_type t = newNode(val);

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

		iterator begin()const{ return head; }
		iterator end()const{ return tail; }
		const_iterator cbegin()const{
		 	return const_iterator(begin());
		}
		const_iterator cend()const{
			return const_iterator(end());
		}
		reverse_iterator rbegin(){
			return reverse_iterator(begin());
		}
		reverse_iterator rend(){
			return reverse_iterator(end());
		}

		iterator insert(iterator position, const value_type& val){
			iterator re;
			if (position == begin()){
				push_front(val);
				re = begin();
			}else if (position == end()){
				iterator ret = position;
				push_back(val);
				re = ret;
			}else{
				link_type t = newNode(val);
				
				t->next = position.node;
				t->prev = position.node->prev;
				t->prev->next = t;
				t->next->prev = t;
				re = --position;
			}

			return re;
		}
		
		template <class InputIterator>
		void insert(iterator position, size_type count, const value_type& val){
			insert_aux(position, count, val, typename std::is_integral<InputIterator>::type());
			for( size_type i = 0; i < count; i++){
				insert(position,val);
			}
		}
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last){
			insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
		}

		iterator erase(iterator position){
			if(position == head){
				pop_front();   
				return head;
			}else{
				link_type prev = position.node->prev;
				prev->next = position.node->next;
				position.node->next->prev = prev;
				deleteNode(position.node);
				return iterator(prev->next);
			}
		}
		iterator erase(iterator first, iterator last){
			iterator re;
			while(first != last){
				re = erase(first++);
			}
			return re;
		}
		void swap(list& x){
			TMPS::swap(head.node,x.head.node);
			TMPS::swap(tail.node,x.tail.node);
		}
		void clear(){
			while(head != tail){
				iterator p = head++;
				deleteNode(p.node);
			}
		}
		
		void splice(iterator position, list& other){
			splice(position, other, other.begin(), other.end());
			other.head.node = other.tail.node;
		}
		void splice(const_iterator position, list& other, iterator i){
			iterator next = i;
			splice(position, other, i, ++next);
		}
		void splice(const_iterator position, list& other, iterator first, iterator last){
			if (first.node == last.node) return;

			link_type tailNode = last.node->prev;
			if(other.head.node == first.node){
				other.head.node = last.node;
				other.head.node->prev = nullptr;
			}else{
				first.node->prev->next = last.node;
				last.node->prev = first.node->prev;
			}
			
			if (position.node == head.node){
				first.node->prev = nullptr;
				tailNode->next = head.node;
				head.node->prev = tailNode;
				head.node = first.node;
			}else{
				position.node->prev->next = first.node;
				first.node->prev = position.node->prev;
				tailNode->next = position.node;
				position.node->prev = tailNode;
			}
		}
		void remove(const value_type& val){
			for(iterator it = begin(); it != end(); ++it){
				if(*it == val) it = erase(it);
				else ++it;
			}
		}
		template <class UnaryPredicate>
		void remove_if(UnaryPredicate pred){
			for (iterator it = begin(); it != end();){
				if (pred(*it)) it = erase(it);
				else ++it;
			}
		}
		void unique(){
			iterator pre = begin();
			iterator p = ++begin();
			while(p != end()){
				if(*pre == *p){
					p = erase(p);
				}else{
					pre++;
					p++;
				}
			}
		}
		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred){
			iterator pre = begin();
			iterator p = ++begin();
			while(p != end()){
				if(binary_pred(*pre, *p)){
					p = erase(p);
				}else{
					pre++;
					p++;
				}
			}
		}
		void merge(list& other){
			iterator this_it = begin();
			iterator other_it = other.begin();
			while(this_it != end() && other_it != end()){
				if(*this_it <= *other_it){
					++this_it;
				}else{
					iterator temp = other_it++;
					splice(this_it, other, temp);
				}
			}
			if(this_it == end()){
				splice(this_it, other, other_it, other.end());
			}
		}
		template <class Compare>
		void merge(list& other, Compare comp){
			iterator this_it = begin();
			iterator other_it = other.begin();
			while(this_it != end() && other_it != end()){
				if(comp(*this_it, *other_it)){
					++this_it;
				}else{
					iterator temp = other_it++;
					splice(this_it, other, temp);
				}
			}
			if(this_it == end()){
				splice(this_it, other, other_it, other.end());
			}
		
		}
		void sort(){
			TMPS::sort(begin(), end());
		}
		template <class Compare>
		void sort(Compare comp){
			TMPS::sort(begin(), end(),comp);	
		}
		void reverse(){
			if(empty() || ++begin() == end()){
				return;
			}
			
			link_type p = head.node;
			do{
				link_type prev = p->prev;
				link_type next = p->next;
				p->prev = next;
				p->next = prev;
				p = p->prev;
			}while(p != head.node);

			head = tail.node->next;
		}
	private:
		void ctorAux(size_type n, const value_type& val, std::true_type){
			head.p = newNode();
			tail.p = head.p;
			while (n--)
				push_back(val);
		}
		template <class InputIterator>
		void ctorAux(InputIterator first, InputIterator last, std::false_type){
			head.p = newNode();
			tail.p = head.p;
			while(first != last)
				push_back(*(first++));
		}

		link_type newNode(const T& val = T()){
			link_type res = list_node_allocator::allocate();

			// list_node_allocator::construct(res, list_node(val, nullptr, nullptr));
			data_allocator::construct((pointer)res, val);
			res->prev = res->next = nullptr;
			
			return res;
		}
		void deleteNode(link_type p){ 
			p->prev = p->next = nullptr;
			data_allocator::destroy((pointer)p);
			list_node_allocator::deallocate(p);
		}

		void insert_aux(iterator position, size_type n, const T& val, std::true_type){
			for (size_type i = n; i != 0; --i){
				position = insert(position, val);
			}
		}
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type){
			for (--last; first != last; --last){
				position = insert(position, *last);
			}
			insert(position, *last);
		}
	public:
		friend void swap(list<T>& x, list<T>& y){
			x.swap(y);
		}
		friend bool operator== (const list<T>& lhs, const list<T>& rhs){
			typename list<T>::iterator lhs_it,rhs_it;
			for(lhs.begin(),rhs_it = rhs.begin(); lhs_it != lhs.end() && rhs_it != rhs.end(); lhs_it++,rhs_it++){
				if(!(*lhs_it == *rhs_it)) break;
			}
			if(lhs_it == lhs.end() && rhs_it == rhs_it){
				return true;
			}
			return false;
			// return lhs.head.node == rhs.head.node && lhs.tail.node == rhs.tail.node;
		}
		friend bool operator!= (const list<T>& lhs, const list<T>& rhs){
			return !(lhs == rhs);
		}
    public:
		list(){
            node = newNode();

			node->next = node;
			node->prev = node;

		    head.node = node;
			tail.node = node;
        }
		explicit list(size_type n, const value_type& val = value_type()){
			ctorAux(n, val, std::is_integral<value_type>());
		}
		template <class InputIterator>
		list(InputIterator first, InputIterator last){
			ctorAux(first, last, std::is_integral<InputIterator>());
		}

		list(const list& l){
			head.node = newNode();
			tail.node = head.node;
			for( link_type node = l.head.node; node != l->tail.node; node = node->next){
				push_back(node->data);
			}
        }

		list& operator = (const list& l){
			if(this != &l){
				list(l).swap(*this);
			}
		}
		~list(){
			while(head != tail) {
				iterator temp = head++;
				deleteNode(temp.node);
			}
			deleteNode(tail.node);
		}
		Alloc get_allocator() const{
			return Alloc();
		}
        // list(const Alloc&);
        // explicit list( size_type count, const T& value = T(),const Allocator& alloc = Allocator()){}
        // template< class InputIt >
        // list( InputIt first, InputIt last, const Allocator& alloc = Allocator() ){}
        // list( const list& other, const Allocator& alloc ){}
        // list( list&& other, const Allocator& alloc ){}
        // list( std::initializer_list<T> init, const Allocator& alloc = Allocator() ){}
    };

}




#endif /* __LIST_H__ */