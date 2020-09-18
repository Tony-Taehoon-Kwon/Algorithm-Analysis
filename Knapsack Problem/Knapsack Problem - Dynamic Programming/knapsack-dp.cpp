#include "knapsack-dp.h"
#include <iostream>
#include <numeric>
#include <algorithm> // std::max

////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value ) 
	: weight(weight), value(value) 
{
}

////////////////////////////////////////////////////////////
Item::Item( Item const& original ) 
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item) {
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item) {
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();

    /* Set up table */
	Table table(W + 1, std::vector<int>(num_items + 1, 0));
	
	for (int i = 1; i <= num_items; ++i)
	{
		for (int w = 1; w <= W; ++w)
		{
			/* if there is a chance for the next item to be included in the bag */
			/* find the optimal way to put the item in the bag */
			/* optimal value would be stored in the table */
			if (w >= items[i - 1].weight)
				table[w][i] = std::max(table[w][i - 1],
					table[w - items[i - 1].weight][i - 1] + items[i - 1].value);
			/* item could not be included in the bag */
			else
				table[w][i] = table[w][i - 1];
		}
	}

	//print final table - for debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 


	//figure out which items are in the bag based on the table
	std::vector<int> bag;

	/* start from the optimized value */
	int res = table[W][num_items];
	for (int i = num_items, w = W; i > 0 && res > 0; --i)
	{
		if (res == table[w][i - 1])
			continue;

		bag.push_back(i - 1);

		/* item value and weight would be deducted */
		res -= items[i - 1].value;
		w   -= items[i - 1].weight;
	}
	return bag;
}

////////////////////////////////////////////////////////////
int valueBag( std::vector<Item> const& items, std::vector<int> const& bag ) {
	std::vector<int>::const_iterator it   = bag.begin(),
		                          it_e = bag.end();

	int accum = 0;
	//std::cout << "Bag ";
	for ( ; it != it_e; ++it) { 
		accum += items[ *it ].value; 
		//std::cout << *it << " ";
	}
	//std::cout << std::endl;
	return accum;
}

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const&, int const&, int, Table& );

////////////////////////////////////////////////////////////
//function to kick start
std::vector<int> knapsackRecMem( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();

    /* Set up table & initialize first row and col with 0 */
	Table table(W + 1, std::vector<int>(num_items + 1, -1));
	for (int i = 0; i <= num_items; ++i) // if capacity is 0, solution is 0 (empty knapsack)
		table[0][i] = 0;
	for (int w = 0; w <= W; ++w) // if item list is empty, solution is 0 (empty knapsack)
		table[w][0] = 0;

	knapsackRecMemAux(items, W, num_items, table);

	//print table - debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { 
            std::cout << items[n].weight << "," << items[n].value<<"   "; 
        }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 

	//figure out which items are in the bag based on the table
	std::vector<int> bag;

	/* start from the optimized value */
	int res = table[W][num_items];
	for (int i = num_items, w = W; i > 0 && res > 0; --i)
	{
		if (res == table[w][i - 1])
			continue;

		bag.push_back(i - 1);

		/* item value and weight would be deducted */
		res -= items[i - 1].value;
		w   -= items[i - 1].weight;
	}
	return bag;
}

////////////////////////////////////////////////////////////
//the real recursive function
int
knapsackRecMemAux( std::vector<Item> const& items, int const& W, int index, Table & table ) {
    
	/* Base case : first row and col -> return table[0][index] or table[W][0] */
	if (index == 0 || W == 0)
		return 0;

	/* if table value is already calculated, return the value */
	if (table[W][index] != -1)
		return table[W][index];

	/* if there is a chance for the next item to be included in the bag */
	if (W >= items[index - 1].weight)
	{
		/* find the optimal way to put the item in the bag */
		/* optimal value would be stored in the table */
		table[W][index] = std::max(knapsackRecMemAux(items, W, index - 1, table),
			knapsackRecMemAux(items, W - items[index - 1].weight, index - 1, table)
			 + items[index - 1].value);
		return table[W][index];
	}
	else
	{
		/* item could not be included in the bag */
		table[W][index] = knapsackRecMemAux(items, W, index - 1, table);
		return table[W][index];
	}
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
