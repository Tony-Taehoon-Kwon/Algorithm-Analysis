#include "perm-jt.h"

PermJohnsonTrotter::PermJohnsonTrotter(int size)
	: perm(), dir(), size(size), largestMobileValue(size)
{
	/* set up the initial permutation in ascending order starting with 1
	   and initialize the direction as well */
	for (int i = 0; i < size; ++i)
	{
		perm.push_back(i + 1);
		dir.push_back(LEFT);
	}
}

bool PermJohnsonTrotter::Next()
{
	largestMobileValue = 0; // has meaning of minus infinity

	/* find the largest mobile value */
	for (int i = 0; i < size; ++i)
	{
		if (GetDirection(i) == LEFT && i != 0)
		{
			/* if the value in position i is greater than adjacent value
			   with the given direction and if it's greater than best so far,
			   then update the largest mobile value*/
			if (perm[i] > perm[i - 1] && perm[i] > largestMobileValue)
				largestMobileValue = perm[i];
		}

		if (GetDirection(i) == RIGHT && i != size - 1)
		{
			/* if the value in position i is greater than adjacent value
			   with the given direction and if it's greater than best so far,
			   then update the largest mobile value*/
			if (perm[i] > perm[i + 1] && perm[i] > largestMobileValue)
				largestMobileValue = perm[i];
		}
	}

	/* if there is no more step to move (no mobile left),
		then return false (terminate) */
	if (largestMobileValue == 0)
		return false;
	
	/* variable to store the index of the largest mobile */
	int pos = GetPermIndex(largestMobileValue);

	/* swap the mobile and the adjacent value with given direction */
	std::swap(perm[pos], perm[pos+GetDirection(pos)]);

	/* switch the direction of all elements
	   which is greater than the current mobile */
	for (int i = 0; i < size; ++i)
	{
		if (perm[i] > largestMobileValue)
		{
			if (GetDirection(i) == RIGHT)
				SetDirection(i, LEFT);
			else if (GetDirection(i) == LEFT)
				SetDirection(i, RIGHT);
		}
	}
	return true;
}

int PermJohnsonTrotter::GetPermIndex(int value) const
{
	for (int i = 0; i < size; ++i)
	{
		if (perm[i] == value)
			return i;
	}
	return -1; // error
}
