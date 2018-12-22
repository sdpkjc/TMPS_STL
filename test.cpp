#include <iostream>
#include "allocator.h"
#include <memory>
#include <vector>
#include "list.h"

using namespace std;

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

class Test
{
private:
    int test_cnt;
    int test_pass;
    std::vector<int,TMPS::allocator<int> > vetorTest;
    std::vector<testA,TMPS::allocator<testA> > vecTestA;
public:
    Test(){
        test_cnt = 0;
        test_pass = 0;
    }
    void test_bool(bool flag,bool result)
    {
        test_cnt++;
        if(flag == result){
            test_pass++;
        }
    }
    void print()
    {
        cout << test_pass << "/" << test_cnt << " (" << (double)test_pass/(double)test_cnt*100 << "%) passed." << endl;
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
    int test_cnt;
    int test_pass;
public:

    bool test_push_back()
    {
        return true;
    }

    void print()
    {
        cout << test_pass << "/" << test_cnt << " (" << (double)test_pass/(double)test_cnt*100 << "%) passed." << endl;
    }
    void test()
    {
        test_bool(true,test_push_back());
        //test_bool(true,test_push_pop());
        //test_bool(true,test_vecTestA_push_back());
        //test_bool(true,test_vecTestA_pop_back());        
        print();
        test_cnt = 0;
        test_pass = 0;
    }
    ListTest(){
        test_cnt = 0;
        test_pass = 0;
    }
    void test_bool(bool flag,bool result)
    {
        test_cnt++;
        if(flag == result){
            test_pass++;
        }
    }
};


int main()
{   
    Test tv;
    tv.test();
    ListTest lt;
    lt.test();

    return 0;
}