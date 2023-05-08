#include <iostream>
#include <thread>
#include "testtools.hpp"
#include "orderedlistquickpop.hpp" 
#include "orderedlistquickpush.hpp"


static int LIST = 2;
static int POP = 1;
static int PUSH = 0;

static double EPSILON = 0.6;

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
            list.Push(arr[i]);
        }
    };

    auto push_func2 = [&](int value_)
    {
        (void)(value_);
        for (int i = 2500; i < 5000; ++i)
        {
            list.Push(arr[i]);
        }
    };

    auto push_func3 = [&](int value_)
    {
        (void)(value_);
        for (int i = 5000; i < 7500; ++i)
        {
            list.Push(arr[i]);
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
        list.Push(array[i]);
    }

    for (int i = 0; i < 5; ++i)
    {
        #ifndef NDEBUG
        std::cout<<list.Pop()<<std::endl;
        #else
        list.Pop();
        #endif
    }   

    int arr[5000] = {0};
    size_t size = 5000;

    InitializeArray(arr, size);

    for (size_t i = 0; i < size; ++i)
    {
        list.Push(arr[i]);
    }
    
    int prev = list.Pop();
    size_t i = 0;
    for (i = 0; i < size - 1; ++i)
    {
        int curr = list.Pop();
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

void EfficiencyTestQuickPop(void)
{
    QuickPopOrderedList<int> pop_list(&Compare);

    int arr[10000] = {0};
    size_t size = 10000;    
    InitializeArray(arr, size);
    auto push_result = MeasurePushPerformanceTime(&pop_list, size, arr);
    auto pop_result = MeasurePopPerformanceTime(&pop_list, size, arr);

    int arr2[1000] = {0};
    push_result.zero();
    pop_result.zero();
    int input_sizes[] = {1000, 5000, 10000, 50000};
    InitializeArray(arr2, input_sizes[0]);
    push_result = MeasurePushPerformanceTime(&pop_list, input_sizes[0], arr2);
    pop_result = MeasurePopPerformanceTime(&pop_list, input_sizes[0], arr2);
    double sum = 0;

    for (int i = 1; i < 4; ++i)
    {
        int *array = new int[input_sizes[i]];
        InitializeArray(array, input_sizes[i]);
        auto next_push_result = MeasurePushPerformanceTime(&pop_list, input_sizes[i], array);
        auto next_pop_result = MeasurePopPerformanceTime(&pop_list, input_sizes[i], array);
        #ifndef NDEBUG
        std::cout<<next_push_result.count()<<" current push result for size "<< input_sizes[i]<<std::endl;
        std::cout<<push_result.count()<<" previous push result for size "<< input_sizes[i - 1]<<std::endl; 
        std::cout<<next_pop_result.count()<<" current pop result for size "<< input_sizes[i]<<std::endl;
        std::cout<<pop_result.count()<<" previous pop result for size "<< input_sizes[i - 1]<<std::endl;
        #endif
        double pop_diff = next_pop_result.count() - pop_result.count();
        if (0 < pop_diff)
        {
            sum += (pop_diff / pop_result.count());
        }
        push_result = next_push_result;
        pop_result = next_pop_result;
        
        delete [] array;
    }

    #ifndef NDEBUG
    std::cout<<"sum is "<<sum<<" avg is "<<sum / (double)3<<std::endl; 
    #endif
    if ((sum / (double)3) <= EPSILON)
    {
        std::cout<<"Success! Quick Pop is maintaining O(1) time complexiy against measures of size : "<<input_sizes[0]<<" "<<input_sizes[1]<<" "<<input_sizes[2]<<" "<<input_sizes[3]<<std::endl;
    }
    else
    {
        std::cout<<"Failure! Quick Pop is not maintaining O(1) time complexiy"<<std::endl;
    }

}

void EfficiencyTestQuickPush(void)
{
    QuickPushOrderedList<int> push_list(&Compare);

    int arr[10000] = {0};
    size_t size = 10000;    
    InitializeArray(arr, size);
    auto push_result = MeasurePushPerformanceTime(&push_list, size, arr);
    auto pop_result = MeasurePopPerformanceTime(&push_list, size, arr);

    int arr2[1000] = {0};
    push_result.zero();
    pop_result.zero();
    int input_sizes[] = {1000, 5000, 10000, 50000};
    InitializeArray(arr2, input_sizes[0]);
    push_result = MeasurePushPerformanceTime(&push_list, input_sizes[0], arr2);
    pop_result = MeasurePopPerformanceTime(&push_list, input_sizes[0], arr2);
    double sum = 0;

    for (int i = 1; i < 4; ++i)
    {
        int *array = new int[input_sizes[i]];
        InitializeArray(array, input_sizes[i]);
        auto next_push_result = MeasurePushPerformanceTime(&push_list, input_sizes[i], array);
        auto next_pop_result = MeasurePopPerformanceTime(&push_list, input_sizes[i], array);
        #ifndef NDEBUG
        std::cout<<next_push_result.count()<<" current push result for size "<< input_sizes[i]<<std::endl;
        std::cout<<push_result.count()<<" previous push result for size "<< input_sizes[i - 1]<<std::endl; 
        std::cout<<next_pop_result.count()<<" current pop result for size "<< input_sizes[i]<<std::endl;
        std::cout<<pop_result.count()<<" previous pop result for size "<< input_sizes[i - 1]<<std::endl;
        #endif
        double push_diff = next_push_result.count() - push_result.count();
        if (0 < push_diff)
        {
            sum += (push_diff / push_result.count());
        }
        push_result = next_push_result;
        pop_result = next_pop_result;
        
        delete [] array;
    }

    #ifndef NDEBUG
    std::cout<<"sum is "<<sum<<" avg is "<<sum / (double)3<<std::endl; 
    #endif
    if ((sum / (double)3) < EPSILON)
    {
        std::cout<<"Success! Quick Push is maintaining O(1) time complexiy against measures of size : "<<input_sizes[0]<<" "<<input_sizes[1]<<" "<<input_sizes[2]<<" "<<input_sizes[3]<<std::endl;
    }
    else
    {
        std::cout<<"Failure! Quick Push is not maintaining O(1) time complexiy"<<std::endl;
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

    EfficiencyTestQuickPop();
    EfficiencyTestQuickPush();


    QuickPushOrderedList<int> push_list(&Compare);
    ThreadSafetyTest(push_list, PUSH);
    QuickPopOrderedList<int> pop_list(&Compare);
    ThreadSafetyTest(pop_list, POP);
    return 0;
}




