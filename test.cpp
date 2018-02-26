// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "testslib.hpp"
#include <array>
#include <vector>
#include <deque>
#include <string>
#include "MyDeque.hpp"

#include<algorithm>

/*****************************************************************************/



/*****************************************************************************/


DECLARE_OOP_TEST(push_back_test)
{

    Deque<int> d1;
	
    size_t size{};

    for (int i = 1; i < 101; i++) 
    {
    
        d1.pushBack(i);
        assert(d1.back() == i);
        assert(d1.size() == ++size);
    }

    
}

DECLARE_OOP_TEST(push_back_test_2)
{
    Deque<int> d1;

    d1.pushBack(1);
    d1.pushBack(2);
    d1.pushBack(3);

    d1.popBack();
    assert(d1.back() == 2);

    d1.popBack();
    assert(d1.back() == 1);
    assert(d1.size() == 1);
}

DECLARE_OOP_TEST(push_test_3)
{
    Deque<int> d1;

    d1.pushFront(1);
    d1.pushBack(2);

    assert(d1.front() == 1);
    assert(d1.back() == 2);

    assert(d1.size() == 2);
}

DECLARE_OOP_TEST(push_test_4)
{
    Deque<int> d1;

    d1.pushBack(1);
    d1.pushFront(2);

    assert(d1.front() == 2);
    assert(d1.back() == 1);
    assert(d1.size() == 2);
}
DECLARE_OOP_TEST(push_test_5)
{
    Deque<int> d1;

    d1.pushFront(1);
    d1.pushFront(2);
    d1.pushBack(3);

    d1.popBack();
    d1.popBack();

    assert( d1.front() == 2) ;
    assert(d1.back() == 2);

    assert(d1.size() == 1);
}

DECLARE_OOP_TEST(push_test_6)
{
    Deque<int> d1;

    d1.pushBack(1);
    d1.pushBack(2);
    d1.pushBack(2);

    d1.popFront();
    d1.popFront();

    assert(d1.front() == d1.back() );
    assert(d1.size() == 1);
}

DECLARE_OOP_TEST(push_test_7) 
{
    Deque<int> d1;

    d1.pushBack(1);
    d1.pushFront(2);
    d1.popBack();
    d1.popBack();

    assert(d1.size() == 0);
}

DECLARE_OOP_TEST(push_test_8)
{
    Deque<int> d1;

    d1.pushFront(1);
    d1.pushFront(2);
    d1.popBack();
    d1.popBack();

    assert(d1.size() == 0);
}

DECLARE_OOP_TEST(push_test_9)
{
    Deque<int> d1;

    d1.pushBack(1);
    d1.pushBack(1);
    d1.pushFront(2);
    d1.popBack();
    d1.popBack();
    d1.popBack();
    assert(d1.size() == 0);
}

DECLARE_OOP_TEST(push_test_10)
{
    Deque<int> d1;

    d1.pushBack(1);
    d1.pushBack(1);
    d1.pushBack(1);
    d1.pushBack(1);
    d1.popBack();
    d1.popBack();
    d1.popBack();
    assert(d1.size() == 1);
}

DECLARE_OOP_TEST(push_test_11)
{
    Deque<int> d1;

    d1.pushBack(1);
    d1.pushFront(1);
    d1.pushBack(1);
    d1.pushFront(1);
    d1.popBack();
    d1.popFront();
    d1.popBack();
    assert(d1.size() == 1);
}




/*****************************************************************************/

