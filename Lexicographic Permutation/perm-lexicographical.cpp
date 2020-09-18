#include "perm-lexicographical.h"
#include <algorithm>
#include <iostream>

bool nextPermLexicographical (std::vector<int> & p) {

	/* find the first increasing pattern */
	int i;
	for (i = p.size() - 2; i >= 0; --i)
	{
		/* find the first element such that p[i] < p[i+1] */
		if (p[i] < p[i + 1])
			break;
	}

	/* if there is no pair to swap,
	then the permutation is in decreasing order */
	if (i == -1)
		return false;

	/* set the smallest index as the first element in the tail */
	int smallestIndexInTail = i + 1;

	/* for all elements in the tail */
	for (unsigned int j = i + 1; j < p.size(); ++j)
	{
		/* find the smallest element p[j] that is greater than p[i] */
		if (p[j] > p[i] && p[j] < p[smallestIndexInTail])
			smallestIndexInTail = j;
	}

	/* swap the p[i] and the smallest element in the tail */
	std::swap(p[i], p[smallestIndexInTail]);

	/* reverse the order of the tail,
	which makes the tail in increasing order */
	std::reverse(p.begin() + i+1, p.end());

	return true;
}
