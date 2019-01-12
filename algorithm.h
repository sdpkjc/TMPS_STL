#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "iterator.h"
#include "functional.h"

namespace TMPS{
    namespace {
        template<class RandomIterator, class BinaryPredicate>
        typename iterator_traits<RandomIterator>::value_type
            mid3(RandomIterator first, RandomIterator last, BinaryPredicate pred){//[first, last]
            auto mid = first + (last + 1 - first) / 2;
            if (pred(*mid, *first)){
                swap(*mid, *first);
            }
            if (pred(*last, *mid)){
                swap(*last, *mid);
            }
            if (pred(*last, *first)){
                swap(*last, *first);
            }
            auto ret = *mid;
            swap(*mid, *(last - 1));//��mid item��λ��Ϊ�ڱ�
            return ret;
        }
        template<class RandomIterator, class BinaryPredicate>
        void bubble_sort(RandomIterator first, RandomIterator last, BinaryPredicate pred){
            auto len = last - first;
            for (auto i = len; i != 0; --i){
                bool swaped = false;
                for (auto p = first; p != (first + i - 1); ++p){
                    if (pred(*(p + 1), *p)){
                        swap(*(p + 1), *p);
                        swaped = true;
                    }
                }
                if (!swaped)
                    break;
            }
        }
    }

    template<class T>
    void swap(T& a, T& b){
        T temp = a;
        a = b;
        b = temp;
    }
    template<class RandomIterator, class BinaryPredicate>
    void sort(RandomIterator first, RandomIterator last){
        sort(first, last, less<typename iterator_traits<RandomIterator>::value_type>());
    }
    
    template<class RandomIterator, class BinaryPredicate>
    void sort(RandomIterator first, RandomIterator last, BinaryPredicate pred){
        if (first >= last || first + 1 == last)
            return;
        if (last - first <= 20)
            return bubble_sort(first, last, pred);
        auto mid = mid3(first, last - 1, pred);
        auto p1 = first, p2 = last - 2;
        while (p1 < p2){
            while (pred(*p1, mid) && (p1 < p2)) ++p1;
            while (!pred(*p2, mid) && (p1 < p2)) --p2;
            if (p1 < p2){
                swap(*p1, *p2);
            }
        }
        swap(*p1, *(last - 2));
        sort(first, p1, pred);
        sort(p1 + 1, last, pred);
    }
}




#endif /* __ALGORITHM_H__ */