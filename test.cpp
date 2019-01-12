#include <iostream>
#include "allocator.h"
#include <memory>
#include <vector>
#include "list.h"
#include <list>
#include <iterator>

// using namespace std;
using std::cout;
using std::endl;
using std::cerr;
using std::string;

using namespace TMPS;

namespace TMPS_Test{
    enum ERROR_CODE{ACCEOTED,TEST_ERROR,OTHER_TEST_ERROR};
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
            }else{
                cerr << test_cnt << " ERROR!" << endl;
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
        ERROR_CODE test_clear()
        {
            testList.clear();
            int arr[] = {10,9,8,7,6,5,4,3,2,1};

            for( int i = 0; i < sizeof(arr)/sizeof(int); i++){
                testList.push_front(arr[i]);
            }
            ERROR_CODE re = ACCEOTED;
            
            if(testList.begin() == testList.end()){
                re = TEST_ERROR;
            }

            testList.clear();
            if(testList.begin() != testList.end()){
                re = OTHER_TEST_ERROR;
            }       
            return re;
        }

        ERROR_CODE test_push_back()
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
                    return TEST_ERROR;
                }
            }
            
    
            if(index != arrSize){
                return TEST_ERROR;
            }

            return ACCEOTED;
        }

        ERROR_CODE test_pop_back()
        {
            testList.clear();
            int arr[] = {1,2,3,4,5,6,7,8,9,10};
            int arrSize = sizeof(arr)/sizeof(int);
            for( int i = 0; i < arrSize; i++){
                testList.push_back(arr[i]);
            }

            int index = 0;
            for( TMPS::list<int>::iterator it = testList.begin(); it != testList.end() && index < arrSize; it++,index++){
                // cout << *it << endl;
                if(*it != arr[index]){
                    return OTHER_TEST_ERROR;
                }
            }
            if(index != arrSize){
                return OTHER_TEST_ERROR;
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
                return TEST_ERROR;
            }
            return ACCEOTED;
        }

        ERROR_CODE test_push_front()
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
                    return TEST_ERROR;
                }
            }
            
            if(index != -1){
                return TEST_ERROR;
            }

            return ACCEOTED;
        }
        ERROR_CODE test_pop_front()
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
                    return OTHER_TEST_ERROR;
                }
            }
            
            if(index != -1){
                return OTHER_TEST_ERROR;
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
                return TEST_ERROR;
            }
            return ACCEOTED;
        }

        ERROR_CODE test_insert()
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
                return TEST_ERROR;
            }
            // 0 1 2 3 4
            it = testList.begin();
            for( int i = 0; i < 5; i++,it++){
                // cout << *it << " "<< i << endl;
                if(*it != i){
                    // cout << *it << i << endl;
                    return TEST_ERROR;
                }    
            }

            return ACCEOTED;
        }
        void print()
        {
            cout << testName << " " << test_pass << "/" << test_cnt << " (" << (double)test_pass/(double)test_cnt*100 << "%) passed." << endl;
        }
        void test_bool(ERROR_CODE result)
        {
            test_cnt++;
            if(ACCEOTED == result){
                test_pass++;
            }else{
                cerr << "X: " << test_cnt << " ERROR!, ERROR_CODE = " << result << endl;
            }
        }
    public:
        ListTest():testName("ListTest")
                ,test_cnt(0)
                ,test_pass(0){ }

        void test()
        {
            test_bool(test_clear());
            test_bool(test_push_front());
            test_bool(test_pop_front());
            test_bool(test_push_back());
            test_bool(test_pop_back());
            test_bool(test_insert());

            // test_bool(true,test_push_pop());
            // test_bool(true,test_vecTestA_push_back());
            // test_bool(true,test_vecTestA_pop_back());        
            print();
            test_cnt = 0;
            test_pass = 0;
        }

    };
}










class A{
public:
    int a,b;
    A(){}
    A(int i):a(i){ cout << i << endl;}
    A(const A& x):a(x.a){cout << "copy " << a << endl;}
    A(const A&& x):a(x.a){cout << "copy&& " << a << endl;}
    
    void f()
    {
        // A x = A(1);
    }
    bool operator==(const A& other)const{
        return a == other.a;
    }
};

int main()
{   
    TMPS_Test::AllocatorTest tv;
    tv.test();
    TMPS_Test::ListTest lt;
    lt.test();

    // std::list<int> xxx;
    // std::list<int>::iterator it = xxx.begin();
    // xxx.push_back(1);
    // xxx.push_back(2);
    // xxx.push_back(3);
    // cout << xxx.size() << endl;

    // xxx.erase(xxx.begin(),xxx.begin());
    // cout << xxx.size() << endl;
    // std::list<int>::const_iterator aaa = xxx.begin();

    // cout << *aaa << "  sss" << endl;
    // // *aaa = 2;
    // cout << *aaa << "  sss" << endl;

    // TMPS::list<int> xlt;
    // xlt.push_back(1);
    // xlt.push_back(2);
    // xlt.push_back(3);
    
    // TMPS::list<int>::const_iterator xlci = xlt.begin();
    
    // cout << "Test_const  " << *xlci << endl;
    // // *xlci = 2;
    
    // cout << "Test_const  " << *xlci << endl;

    // TMPS::list<A> xlt_class;

    // xlt_class.push_back(A(1));
    // xlt_class.push_back(A(2));
    // xlt_class.push_back(A(3));
    // TMPS::list<A>::const_iterator xlci_class = xlt_class.begin();
    // // xlci_class->a = 1;

    // TMPS::list<A> xlt_class_B;
    // xlt_class_B.push_back(A(1));
    // xlt_class_B.push_back(A(2));
    // xlt_class_B.push_back(A(3));
    // xlt_class_B.push_back(A(4));
    // xlt_class_B.push_back(A(5));
    // xlt_class_B.push_back(A(6));

    // TMPS::list<A>::iterator itxx;

    // for( itxx = xlt_class_B.begin(); itxx != xlt_class_B.end(); itxx++){
    //     cout << itxx->a << endl;
    // }
    // xlt_class_B.reverse();
    // cout << endl << endl;
    // for( itxx = xlt_class_B.begin(); itxx != xlt_class_B.end(); itxx++){
    //     cout << itxx->a << endl;
    // }
    // if(xlt_class == xlt_class_B){

    // }
    // swap(xlt_class,xlt_class_B);
    
    // cout << endl << sizeof(std::list<A>) << endl << sizeof(TMPS::list<A>) << endl;
    
    // TMPS::list<A> xx;
    // xx.push_back(A(1));
    // xx.push_back(A(2));
    // xx.push_back(A(3));
    // xx.push_back(A(4));
    // xx.push_back(A(5));
    

    // for( TMPS::list<A>::iterator itxx = xx.begin(); itxx != xx.end(); itxx++){
    //     cout << itxx->a << endl;
    // }
    // cout << endl;
    // for( TMPS::list<A>::reverse_iterator itxx = xx.rend(); itxx != xx.rbegin(); itxx++){
    //     cout << itxx->a << endl;
    // }

    return 0;
}