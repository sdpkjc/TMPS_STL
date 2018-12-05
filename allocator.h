#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "alloc.h"

#include <new>      // placement new
#include <cstddef>  // prtdiff_t, size_t

namespace TMPS_STL{
	template<class T>
	class allocator{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
	public:
		static value_type *allocate(){
			return static_cast<pointer>(alloc::allocate(sizeof(value_type)));
		}
		static pointer allocate(size_type n){
			if (0 == n) return 0;
			return static_cast<pointer>(alloc::allocate(sizeof(value_type) * n));
		}

		static void deallocate(pointer ptr){
			alloc::deallocate(static_cast<void *>(ptr), sizeof(value_type));
		}
		static void deallocate(pointer ptr, size_type n){
			if (0 == n) return;
			alloc::deallocate(static_cast<void *>(ptr), sizeof(value_type) * n);
		}

		static void construct(pointer ptr){
			new(ptr)value_type();
		}
		static void construct(pointer ptr, const_reference value){
			new(ptr)value_type(value);
		}

		static void destroy(T *ptr){
			ptr->~T();
		}
		static void destroy(pointer first, pointer last){
			for ( ; first != last; ++first){
				first->~value_type();
			}
		}
	};
}

#endif /* __ALLOCATOR_H__ */