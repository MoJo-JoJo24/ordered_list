#include <iostream>
#include "orderedlistquickpop.hpp" 
#include "orderedlistquickpush.hpp"
#include "testtools.hpp"


static double EPSILON = 0.6;

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
    int input_sizes[] = {1000, 5000, 10000, 50000, 100000};
    InitializeArray(arr2, input_sizes[0]);
    push_result = MeasurePushPerformanceTime(&pop_list, input_sizes[0], arr2);
    pop_result = MeasurePopPerformanceTime(&pop_list, input_sizes[0], arr2);
    double sum = 0;

    for (int i = 1; i < 5; ++i)
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
    std::cout<<"sum is "<<sum<<" avg is "<<sum / (double)4<<std::endl; 
    #endif
    if ((sum / (double)4) <= EPSILON)
    {
        std::cout<<"Success! Quick Pop is maintaining O(1) time complexiy against measures of size : "<<input_sizes[0]<<" "<<input_sizes[1]<<" "<<input_sizes[2]<<" "<<input_sizes[3]<<" "<<input_sizes[4]<<std::endl;
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
    int input_sizes[] = {1000, 5000, 10000, 50000, 100000};
    InitializeArray(arr2, input_sizes[0]);
    push_result = MeasurePushPerformanceTime(&push_list, input_sizes[0], arr2);
    pop_result = MeasurePopPerformanceTime(&push_list, input_sizes[0], arr2);
    double sum = 0;

    for (int i = 1; i < 5; ++i)
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
    std::cout<<"sum is "<<sum<<" avg is "<<sum / (double)4<<std::endl; 
    #endif
    if ((sum / (double)4) < EPSILON)
    {
        std::cout<<"Success! Quick Push is maintaining O(1) time complexiy against measures of size : "<<input_sizes[0]<<" "<<input_sizes[1]<<" "<<input_sizes[2]<<" "<<input_sizes[3]<<" "<<input_sizes[4]<<std::endl;
    }
    else
    {
        std::cout<<"Failure! Quick Push is not maintaining O(1) time complexiy"<<std::endl;
    }
}

int main()
{
    EfficiencyTestQuickPop();
    EfficiencyTestQuickPush();
    return 0;
}




