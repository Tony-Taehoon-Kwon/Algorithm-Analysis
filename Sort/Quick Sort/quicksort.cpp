#include "quicksort.h"
#include <algorithm>

int partition(int* a, unsigned first, unsigned last)
{
	/* pick the last element as a pivot */
	int i = first;
	int pivot = a[last];
	
	/* for all array elements except pivot */
	for (unsigned j = first; j < last; ++j)
	{
		/* make the first part of array with smaller than pivot,
		   second part of array with larger than pivot */
		if (a[j] < pivot)
		{
			std::swap(a[j], a[i]);
			++i;
		}
	}
	/* position the pivot */
	std::swap(a[i], a[last]);
	return i;
}

void quicksort(int* a, unsigned b, unsigned e)
{
	/* if array size is greater or equal to 2 */
	if (e - b >= 2)
	{
		/* get the pivot and do the partition */
		int pivot = partition(a, b, e - 1);

		quicksort(a, b, pivot); // this does not include pivot
		quicksort(a, pivot + 1, e);
	}
}