#ifndef PERM_JOHNSON_TROTTER_H
#define PERM_JOHNSON_TROTTER_H
#include <vector>

#define LEFT -1
#define RIGHT 1

class PermJohnsonTrotter {
	
	public:
		PermJohnsonTrotter(int size);
		bool Next();
		std::vector<int> const& Get() const { return perm; }

/* Each element in the permutation vector 'perm' has the value of 1,2,3,4,5,....
   and each of its direction is stored in the direction vector 'dir'.
   The element of 'dir' vector has never been swaped,
   so the direction of each 'perm' vector could be found as dir[perm[i]-1]
   since the index of dir corresponds to the (value of perm -1) */
#define GetDirection(i)		dir[perm[i]-1]
#define SetDirection(i, x)  dir[perm[i]-1] = x

	// helper function
	private:
		int GetPermIndex(int value) const;

	private:
		std::vector<int> perm;
		std::vector<int> dir;
		int size;
		int largestMobileValue;
};
#endif
