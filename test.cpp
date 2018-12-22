#include <iostream>
#include "allocator.h"
#include <memory>
#include <vector>
#include "list.h"
#include <list>

using namespace std;
namespace TMPS_Test{
    class testA
    {
    public:
        int cnt;
        int *arr;
        testA()
        {
            cnt = 11;
            arr = new int[10];
            arr[0] = 11;
        }
        ~testA()
        {
            delete arr;
        }
        testA(const testA& t) {
            cnt = t.cnt;
            arr = new int [10];
            arr[0] = t.arr[0];
        }
    };

    class AllocatorTest
    {
    private:
        string testName;
        int test_cnt;
        int test_pass;
        std::vector<int,TMPS::allocator<int> > vetorTest;
        std::vector<testA,TMPS::allocator<testA> > vecTestA;
        void test_bool(bool flag,bool result)
        {
            test_cnt++;
            if(flag == result){
                test_pass++;
            }
        }
        void print()
        {
            cout << testName << " " << test_pass << "/" << test_cnt << " (" << (double)test_pass/(double)test_cnt*100 << "%) passed." << endl;
        }
        bool test_push_back()
        {
            int a[100];
            for( int i = 0; i < 100; i++){
                a[i] = i;
                vetorTest.push_back(a[i]);
            }
            bool re = true;
            for( int i = 0; i < 100; i++){
                if(a[i] != vetorTest[i]){
                    re = false;
                    break;
                }
            }
            vetorTest.clear();
            return re;
        }

        bool test_push_pop()
        {
            int a[100];
            for( int i = 0; i < 100; i++){
                a[i] = i;
                vetorTest.push_back(a[i]);
            }
            bool re = true;
            for( int i = 0; i < 100; i++){
                if(vetorTest.size() != 100 - i){
                    re = false;
                    break;
                }
                vetorTest.pop_back();
            }
            vetorTest.clear();
            return re;
        }

        bool test_vecTestA_push_back()
        {
            bool re = true;
            for( int i = 0; i < 100; i++){
                testA a;
                vecTestA.push_back(a);
            }

            for( int i = 0; i < 100; i++){
                if(vecTestA[i].cnt != 11 || vecTestA[i].arr[0] != 11){
                    re = false;
                    break;
                }
            }

            vecTestA.clear();
            
            return re;
        }

        bool test_vecTestA_pop_back()
        {
            bool re = true;
            for( int i = 0; i < 100; i++){
                testA a;
                vecTestA.push_back(a);
            }

            for( int i = 0; i < 100; i++){

                if(vecTestA.size() != 100 - i){
                    re = false;
                    break;
                }
                vecTestA.pop_back();
            }

            vecTestA.clear();
            
            return re;
        }
    public:
        AllocatorTest():testName("AllocatorTest")
                ,test_cnt(0)
                ,test_pass(0){ }

        void test()
        {
            test_bool(true,test_push_back());
            test_bool(true,test_push_pop());
            test_bool(true,test_vecTestA_push_back());
            test_bool(true,test_vecTestA_pop_back());        
            print();
            test_cnt = 0;
            test_pass = 0;
        }

    };


    class ListTest{
    private:
        TMPS::list<int> testList;
        string testName;
        int test_cnt;
        int test_pass;

    private:
        bool test_clear()
        {
            testList.clear();
            int arr[] = {10,9,8,7,6,5,4,3,2,1};

            for( int i = 0; i < sizeof(arr)/sizeof(int); i++){
                testList.push_front(arr[i]);
            }
            bool re = true;
            if(testList.begin() == testList.end()){
                re = false;
            }

            testList.clear();
            if(testList.begin() != testList.end()){
                re = false;
            }       
            return re;
        }

        bool test_push_back()
        {
            testList.clear();
            int arr[] = {1,2,3,4,5,6,7,8,9,10};
            int arrSize = sizeof(arr)/sizeof(int);
            for( int i = 0; i < arrSize; i++){
                testList.push_back(arr[i]);
            }
            // if(testList.begin() == testList.end()){
            //     cout << "ssss" << endl;
            // }
            int index = 0;
            for( TMPS::list<int>::iterator it = testList.begin(); it != testList.end() && index < arrSize; it++,index++){
                // cout << *it << endl;
                if(*it != arr[index]){
                    return false;
                }
            }
            
    
            if(index != arrSize){
                return false;
            }

            return true;
        }

        bool test_pop_back()
        {
            testList.clear();
            int arr[] = {1,2,3,4,5,6,7,8,9,10};
            int arrSize = sizeof(arr)/sizeof(int);
            for( int i = 0; i < arrSize; i++){
                testList.push_back(arr[i]);
            }
            
            for( int i = 0; i < arrSize; i++){
                // for( TMPS::list<int>::iterator it = testList.begin(); it != testList.end(); it++){
                //     cout << *it << " ";
                // }
                // cout << endl;
                testList.pop_back();
            }
            // testList.pop_front();
            // TMPS::list<int>::iterator it = testList.begin();
            // cout << *it << endl;
            if(testList.begin() != testList.end()){
                return false;
            }
            return true;
        }

        bool test_push_front()
        {
            testList.clear();
            int arr[] = {10,9,8,7,6,5,4,3,2,1};
            int arrSize = sizeof(arr)/sizeof(int);
            for( int i = 0; i < arrSize; i++){
                testList.push_front(arr[i]);
            }
            
            int index = arrSize - 1;
            for( TMPS::list<int>::iterator it = testList.begin(); it != testList.end() && index >= 0; it++,index--){
                if(*it != arr[index]){
                    // cout << *it << endl;
                    return false;
                }
            }
            
            if(index != -1){
                return false;
            }

            return true;
        }
        bool test_pop_front()
        {
            testList.clear();
            int arr[] = {10,9,8,7,6,5,4,3,2,1};
            int arrSize = sizeof(arr)/sizeof(int);
            for( int i = 0; i < arrSize; i++){
                testList.push_front(arr[i]);
            }
            
            for( int i = 0; i < arrSize; i++){
                // for( TMPS::list<int>::iterator it = testList.begin(); it != testList.end(); it++){
                //     cout << *it << " ";
                // }
                // cout << endl;
                testList.pop_front();
            }
            // testList.pop_front();
            // TMPS::list<int>::iterator it = testList.begin();
            // cout << *it << endl;
            if(testList.begin() != testList.end()){
                return false;
            }
            return true;
        }


        bool test_insert()
        {
            testList.clear();

            testList.insert(testList.begin(),1);
            testList.insert(testList.end(),3);
            testList.insert(testList.end(),4);
            testList.insert(testList.begin(),0);
            
            TMPS::list<int>::iterator it = testList.begin();
            it++;
            it++;
            //it++;

            it = testList.insert(it,2);
            if(*it != 2){
                // cout << "2222" << endl;
                return false;
            }
            // 0 1 2 3 4
            it = testList.begin();
            for( int i = 0; i < 5; i++,it++){
                // cout << *it << " "<< i << endl;
                if(*it != i){
                    // cout << *it << i << endl;
                    return false;
                }    
            }

            return true;
        }
        void print()
        {
            cout << testName << " " << test_pass << "/" << test_cnt << " (" << (double)test_pass/(double)test_cnt*100 << "%) passed." << endl;
        }
        void test_bool(bool flag,bool result)
        {
            test_cnt++;
            if(flag == result){
                test_pass++;
            }
        }
    public:
        ListTest():testName("ListTest")
                ,test_cnt(0)
                ,test_pass(0){ }

        void test()
        {
            test_bool(true,test_clear());
            test_bool(true,test_push_front());
            test_bool(true,test_pop_front());
            test_bool(true,test_push_back());
            test_bool(true,test_pop_back());
            test_bool(true,test_insert());
            // test_bool(true,test_push_pop());
            // test_bool(true,test_vecTestA_push_back());
            // test_bool(true,test_vecTestA_pop_back());        
            print();
            test_cnt = 0;
            test_pass = 0;
        }

    };
}


int main()
{   
    TMPS_Test::AllocatorTest tv;
    tv.test();
    TMPS_Test::ListTest lt;
    lt.test();

    return 0;
}