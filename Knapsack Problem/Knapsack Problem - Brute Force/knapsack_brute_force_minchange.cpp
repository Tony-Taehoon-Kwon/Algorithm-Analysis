#include "knapsack_brute_force_minchange.h"

////////////////////////////////////////////////////////////////////////////////
std::pair< bool, std::pair< bool, int > > GreyCode::Next() 
{
	/* get the grey code */
	int currGreyCode = DeciToGrey(currentStep);
	int nextGreyCode = DeciToGrey(++currentStep);

	/* get the difference between next and current grey code */
	int difference = nextGreyCode - currGreyCode;

	/* which position is modified */
	int pos = 0;

	/* get the position to modify */
	FindPosition(std::abs(difference), pos);

	/* is true if new value is 1 (add item), false otherwise */
	bool add = false;

	if (difference > 0)
		add = true;

	/* is this the last permutation */
	bool last = false;

	/* if this is the last permutation */
	if (currentStep >= termValue)
		last = true;

    return std::make_pair( !last, std::make_pair( add, pos ) );
}

int GreyCode::DeciToGrey(int n)
{
	return n ^ (n >> 1);
}

void GreyCode::FindPosition(int difference, int& position)
{
	if (difference == 1)
		return;

	FindPosition(difference / 2, ++position);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<bool> knapsack_brute_force( std::vector<Item> const& items, Weight const& W )
{
	/* bag that contains items */
	std::vector<bool> bag(items.size(), false);

	/* best bag that contains most valuable items */
	std::vector<bool> bestBag(items.size(), false);

	GreyCode gc(items.size());
	bool go = true;

	/* variable to store sum of weight and value of items */
	Weight totalWeight;
	int totalValue = 0;
	int bestValue = 0;

	while (go)
	{
		std::pair<bool, std::pair<bool, int>> r = gc.Next();
		go = r.first;
		bool add = r.second.first;
		int  pos = r.second.second;

		if (add)
		{
			/* add item and update weight and value */
			bag[pos] = true;
			totalWeight += items[pos].GetWeight();
			totalValue += items[pos].GetValue();
		}
		else
		{
			/* delete item and update weight and value */
			bag[pos] = false;
			totalWeight -= items[pos].GetWeight();
			totalValue -= items[pos].GetValue();
		}

		/* if total weight is less than limit weight
		   and total value is greater than best value */
		if (totalWeight <= W && totalValue > bestValue)
		{
			/* set the best value and best bag */
			bestValue = totalValue;
			bestBag = bag;
		}
	}
	return bestBag;
}
