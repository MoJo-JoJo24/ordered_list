#include <iostream>
#include <thread>
#include "testtools.hpp"
#include "orderedlistquickpop.hpp" 
#include "orderedlistquickpush.hpp"


static int LIST = 2;
static int POP = 1;
static int PUSH = 0;

void ThreadSafetyTest(OrderedList<int> &list, int pop_)
{    
    int arr[10000] = {0};
    size_t size = 10000;    
    std::thread array[4];
    InitializeArray(arr, size);
    auto push_func1 = [&](int value_)
    {
        (void)(value_);
        for (int i = 0; i < 2500; ++i)
        {
            list.Push(&arr[i]);
        }
    };

    auto push_func2 = [&](int value_)
    {
        (void)(value_);
        for (int i = 2500; i < 5000; ++i)
        {
            list.Push(&arr[i]);
        }
    };

    auto push_func3 = [&](int value_)
    {
        (void)(value_);
        for (int i = 5000; i < 7500; ++i)
        {
            list.Push(&arr[i]);
        }
    };

    auto pop_func = [&](int value_)
    {
        (void)(value_);
        for (int i = 0; i < 2000; ++i)
        {
            list.Pop();
        }
    };

    array[1] = std::thread(push_func1, 0);
    array[2] = std::thread(push_func2, 0);
    array[3] = std::thread(push_func3, 0);
    array[0] = std::thread(pop_func, 0);
       
    for (size_t i = 0; i < 4; ++i)
    {
        if (array[i].joinable())
        {
            array[i].join();
        }
    }

    if (pop_)
    {
        std::cout<<"Thread Safety test for array size: "<<size<<" Quick Pop is successful!"<<std::endl;
    }
    else
    {
        std::cout<<"Thread Safety test for array size: "<<size<<" Quick Push is successful!"<<std::endl;    
    }
}


void FunctionalityTest(OrderedList<int> &list, int type)
{
    int array[5] = {1, 5, 3, 2, 7};

    for (int i = 0; i < 5; ++i)
    {
        list.Push(&array[i]);
    }

    for (int i = 0; i < 5; ++i)
    {
        #ifndef NDEBUG
        std::cout<<*list.Pop()<<std::endl;
        #else
        list.Pop();
        #endif
    }   

    int arr[5000] = {0};
    size_t size = 5000;

    InitializeArray(arr, size);

    for (size_t i = 0; i < size; ++i)
    {
        list.Push(&arr[i]);
    }
    
    int prev = *list.Pop();
    size_t i = 0;
    for (i = 0; i < size - 1; ++i)
    {
        int curr = *list.Pop();
        if (curr > prev)
        {
            if (POP == type)
            {
                std::cout<<"Quick Pop Failed! List not in order for size "<<size<<std::endl;
            }
            else if (PUSH == type)
            {
                std::cout<<"Quick Push Failed! List not in order for size "<<size<<std::endl;
            }
            else
            {
                std::cout<<"OrderedList Failed! List not in order for size "<<size<<std::endl;
            }            
            
            break;
        }
        prev = curr;
    }
    if (size - 1 == i)
    {
        if (POP == type)
        {
            std::cout<<"Success! Quick Pop List is in order for size "<<size<<std::endl;
        }
        else if (PUSH == type)
        {
            std::cout<<"Success! Quick Push List is in order for size "<<size<<std::endl;
        }
        else
        {
            std::cout<<"Success! OrderedList List is in order for size "<<size<<std::endl;
        }
    }    
}

int main()
{
    OrderedList<int> list(&Compare);
    QuickPushOrderedList<int> push_(&Compare);
    QuickPopOrderedList<int> pop_(&Compare);
    FunctionalityTest(list, LIST);
    FunctionalityTest(push_, PUSH);
    FunctionalityTest(pop_, POP);

    QuickPushOrderedList<int> push_list(&Compare);
    ThreadSafetyTest(push_list, PUSH);
    QuickPopOrderedList<int> pop_list(&Compare);
    ThreadSafetyTest(pop_list, POP);
    return 0;
}




