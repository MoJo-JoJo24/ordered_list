#ifndef __TESTTOOL_HPP__
#define __TESTTOOL_HPP__
#include <iostream>
#include <stdlib.h> /* rand, srand, qsort */
#include <chrono> //  chrono::steady_clock
#include "orderedList.hpp"




template <class T>
void PushToList(OrderedList<T> *list, size_t size, T *array)
{
    for (size_t i = 0; i < size; ++i)
    {
        list->Push(&array[i]);
    }    
}


template <class T>
std::chrono::nanoseconds MeasurePushPerformanceTime(OrderedList<T> *list, size_t size, T *array)
{
    std::chrono::nanoseconds sum_result(0);

    for (size_t i = 0; i < size; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        list->Push(&array[i]);
        auto end = std::chrono::high_resolution_clock::now();
        sum_result += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    std::chrono::nanoseconds avg_result = sum_result / size ;

    return avg_result;
}


template <class T>
std::chrono::nanoseconds MeasurePopPerformanceTime(OrderedList<T> *list, size_t size, T *array)
{
    (void)array;
    std::chrono::nanoseconds sum_result(0);

    for (size_t i = 0; i < size; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        list->Pop();
        auto end = std::chrono::high_resolution_clock::now();
        sum_result += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    std::chrono::nanoseconds avg_result = sum_result / size ;

    return avg_result;
}

inline int Compare(const int* a, const int* b)
{
    if (nullptr == a)
    {
        return -1;
    }
    else if (nullptr == b)
    {
        return 1;
    }
    else
    {
        return *a - *b;
    }
}

void InitializeArray(int *array, size_t size);




#endif /* __TESTTOOL_HPP__ */
