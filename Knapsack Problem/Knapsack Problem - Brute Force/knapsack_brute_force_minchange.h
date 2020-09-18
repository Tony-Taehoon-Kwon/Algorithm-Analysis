#ifndef KNAPSACK_BRUTE_FORCE_MINCHANGE
#define KNAPSACK_BRUTE_FORCE_MINCHANGE
#include <utility>   // std::pair
#include <vector>
#include <bitset>
#include "definitions.h"

class GreyCode {
    
	private:
		int s;
		int currentStep;
		int termValue;

    public:
        GreyCode( int s ) : s(s), currentStep(0), termValue((1 << s) -1) {}
        // first subset is empty, this function tells what's next 2^n-1 subsets
        // ret value: is last, is add, positin
        std::pair< bool, std::pair< bool, int > > Next();

	// helper function
	private:
		int DeciToGrey(int n);
		void FindPosition(int difference, int& position);
};

// brute-force knapsack
std::vector<bool> knapsack_brute_force( std::vector<Item> const& items, Weight const& W );
#endif
