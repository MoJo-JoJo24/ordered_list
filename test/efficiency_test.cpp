#include <iostream>
#include "orderedlistquickpop.hpp" 
#include "orderedlistquickpush.hpp"
#include "testtools.hpp"


static double EPSILON = 0.05;

void EfficiencyTestQuickPop(void)
{
    QuickPopOrderedList<int> pop_list(&Compare);

    int arr[100000] = {0};
    size_t size = 100000;    
    InitializeArray(arr, size);
    auto push_result = MeasurePushPerformanceTime(&pop_list, size, arr);
    auto pop_result = MeasurePopPerformanceTime(&pop_list, size, arr);

    int arr2[10000] = {0};
    push_result.zero();
    pop_result.zero();
    int input_sizes[] = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    InitializeArray(arr2, input_sizes[0]);
    push_result = MeasurePushPerformanceTime(&pop_list, input_sizes[0], arr2);
    pop_result = MeasurePopPerformanceTime(&pop_list, input_sizes[0], arr2);
    double sum = 0;

    for (int i = 1; i < 10; ++i)
    {
        std::cout<<" QuickPop running size "<< input_sizes[i]<<std::endl;
        int *array = new int[input_sizes[i]];
        InitializeArray(array, input_sizes[i]);
        auto next_push_result = MeasurePushPerformanceTime(&pop_list, input_sizes[i], array);
        auto next_pop_result = MeasurePopPerformanceTime(&pop_list, input_sizes[i], array);
        #ifndef NDEBUG
        std::cout<<next_push_result.count()<<" QuickPop current push result for size "<< input_sizes[i]<<std::endl;
        std::cout<<push_result.count()<<" QuickPop previous push result for size "<< input_sizes[i - 1]<<std::endl; 
        std::cout<<next_pop_result.count()<<" QuickPop current pop result for size "<< input_sizes[i]<<std::endl;
        std::cout<<pop_result.count()<<" QuickPop previous pop result for size "<< input_sizes[i - 1]<<std::endl;
        #endif
        double pop_diff = next_pop_result.count() - pop_result.count();

        sum += (pop_diff / pop_result.count());
        
        push_result = next_push_result;
        pop_result = next_pop_result;
        delete [] array;
    }

    #ifndef NDEBUG
    std::cout<<"sum is "<<sum<<" avg is "<<sum / (double)9<<std::endl; 
    #endif
    if ((sum / (double)9) <= EPSILON)
    {
        std::cout<<"Success! Quick Pop is maintaining O(1) complexity with avg %% change of "<<sum / (double)9<<std::endl;
    }
    else
    {
        std::cout<<"Failure! Quick Pop is not maintaining O(1) time complexiy with avg %% change of "<<sum / (double)9<<std::endl;
    }

}

void EfficiencyTestQuickPush(void)
{
    QuickPushOrderedList<int> push_list(&Compare);

    int arr[100000] = {0};
    size_t size = 100000;    
    InitializeArray(arr, size);
    auto push_result = MeasurePushPerformanceTime(&push_list, size, arr);
    auto pop_result = MeasurePopPerformanceTime(&push_list, size, arr);

    int arr2[10000] = {0};
    push_result.zero();
    pop_result.zero();
    int input_sizes[] = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    InitializeArray(arr2, input_sizes[0]);
    push_result = MeasurePushPerformanceTime(&push_list, input_sizes[0], arr2);
    pop_result = MeasurePopPerformanceTime(&push_list, input_sizes[0], arr2);
    double sum = 0;

    for (int i = 1; i < 10; ++i)
    {
        std::cout<<" QuickPush running size "<< input_sizes[i]<<std::endl;
        int *array = new int[input_sizes[i]];
        InitializeArray(array, input_sizes[i]);
        auto next_push_result = MeasurePushPerformanceTime(&push_list, input_sizes[i], array);
        auto next_pop_result = MeasurePopPerformanceTime(&push_list, input_sizes[i], array);
        #ifndef NDEBUG
        std::cout<<next_push_result.count()<<" QuickPush current push result for size "<< input_sizes[i]<<std::endl;
        std::cout<<push_result.count()<<" QuickPush previous push result for size "<< input_sizes[i - 1]<<std::endl; 
        std::cout<<next_pop_result.count()<<" QuickPush current pop result for size "<< input_sizes[i]<<std::endl;
        std::cout<<pop_result.count()<<" QuickPush previous pop result for size "<< input_sizes[i - 1]<<std::endl;
        #endif
        double push_diff = next_push_result.count() - push_result.count();

        sum += (push_diff / push_result.count());
        
        push_result = next_push_result;
        pop_result = next_pop_result;

        delete [] array;
    }

    #ifndef NDEBUG
    std::cout<<"sum is "<<sum<<" avg is "<<sum / (double)11<<std::endl; 
    #endif
    if ((sum / (double)9) < EPSILON)
    {
        std::cout<<"Success! Quick Push is maintaining O(1) complexity with avg %% change of "<<sum / (double)9<<std::endl;
    }
    else
    {
        std::cout<<"Failure! Quick Push is not maintaining O(1) time complexiy with avg %% change of "<<sum / (double)9<<std::endl;
    }
}

int main()
{
    EfficiencyTestQuickPop();
    EfficiencyTestQuickPush();
    return 0;
}




