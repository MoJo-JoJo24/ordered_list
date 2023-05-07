#include <time.h> /* time */
#include "testtools.hpp"




void InitializeArray(int *array, size_t size)
{
    int lower = 1, upper = 1000;

    srand(time(0));   
	
    for (size_t i = 0; i < size; ++i)
	{
		if (0 == i % 2)
		{
			array[i] = ((rand() % (upper - lower + 1)) + lower) * -1;
		}
		else
		{
			array[i] = ((rand() % (upper - lower + 1)) + lower);			
		}		
	}    
}



