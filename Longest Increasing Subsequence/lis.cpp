#include "lis.h"
#include <algorithm>

int lower_bound(const std::vector<int>& table, int begin, int end, 
				int value, const std::vector<int>& sequence)
{
	/* binary search to get the lower bound */
	while (end - begin > 1)
	{
		int middle = (begin + end) / 2;
		if (sequence[table[middle]] >= value)
			end = middle;
		else
			begin = middle;
	}

	if (sequence[table[begin]] >= value)
		return begin;
	else
		return end;
}

std::vector<unsigned> 
longest_increasing_subsequence( std::vector<int> const& sequence ) {
    
	/* table for intermediate LIS index */
	std::vector<int> t(sequence.size(), 0);
	/* table for backtrack the LIS index */
	std::vector<int> prev(sequence.size(), -1);

    /* main logic */
	int length = 1;
	for (unsigned i = 1; i < sequence.size(); ++i)
	{
		/* optimization for the smallest value */
		if (sequence[i] < sequence[t[0]])
		{
			t[0] = i;
		}
		/* if given value of the sequence is the largest value 
		   among the intermediate LIS, then, extend the list */
		else if (sequence[i] > sequence[t[length - 1]])
		{
			/* keep track of previous index */
			prev[i] = t[length - 1];
			/* extend */
			t[length++] = i;
		}
		else
		{
			/* if given value of the sequence is in between,
			   then, get the lower bound, update the intermediate LIS,
			   and keep track of potential candidate */
			int lb_i = lower_bound(t, 0, length, sequence[i], sequence);
			if (lb_i > 0)
				prev[i] = t[lb_i - 1];
			/* update */
			t[lb_i] = i;
		}
	}

    /* reconstruct subsequence */
	/* vector of indices corresponding to the LIS */
    std::vector<unsigned> answer;

	/* get the answer by backtracking the LIS */
	for (int i = t[length - 1]; i >= 0; i = prev[i])
		answer.push_back(i);

	/* since it's the backtracked LIS, reverse the list */
	std::reverse(answer.begin(), answer.end());

    return answer;
}
