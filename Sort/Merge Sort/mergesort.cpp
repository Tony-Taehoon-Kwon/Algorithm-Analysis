#include "mergesort.h"

void sortAndCombine(int* origianl, unsigned p, unsigned median, unsigned r)
{
	int sizeOfFirstHalf  = median - p + 1;
	int sizeOfSecondHalf = r - median;

	/* allocate 2 arrays of sizes half of the original array */
	int* firstHalf  = new int[sizeOfFirstHalf];
	int* secondHalf = new int[sizeOfSecondHalf];
	
	/* loop variable */
	int i, j, k;

	/* copy the first half of the original array into the first,
	   and second half into the second */
	for (i = 0; i < sizeOfFirstHalf; ++i)
		firstHalf[i] = origianl[p + i];
	for (j = 0; j < sizeOfSecondHalf; ++j)
		secondHalf[j] = origianl[median + 1 + j];

	/* two point traversal, compare two elements in each half array */
	for (i = j = 0, k = p; i < sizeOfFirstHalf && j < sizeOfSecondHalf; ++k)
	{
		if (firstHalf[i] <= secondHalf[j])
		{
			origianl[k] = firstHalf[i];
			++i;
		}
		else
		{
			origianl[k] = secondHalf[j];
			++j;
		}
	}

	/* copy the rest of the array */
	/* note that only one of these loops is non-empty */
	for (; i < sizeOfFirstHalf; ++i, ++k)
		origianl[k] = firstHalf[i];
	for (; j < sizeOfSecondHalf; ++j, ++k)
		origianl[k] = secondHalf[j];

	delete[] firstHalf;
	delete[] secondHalf;
}

void mergesort(int* a, unsigned p, unsigned r)
{
	if (p < r)
	{
		/* median index to split the array */
		int median = (p + r) / 2;

		mergesort(a, p, median);
		mergesort(a, median + 1, r);

		sortAndCombine(a, p, median, r);
	}
}