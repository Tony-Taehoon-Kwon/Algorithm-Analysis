#include "tsp.h"
#include <iostream>
#include <fstream> // ifstream
#include <map> // multimap

int GetLowerBound(const MAP& distance, int numOfCities,
	const std::vector<bool>& visited, int currentCityIndex)
{
	int lower_bound = 0;

	/* i : next city to move */
	for (int i = 0; i < numOfCities; ++i)
	{
		/* if the city is not visited yet */
		if (visited[i] == false)
		{
			/* set the min distance with the distance from current city to the possible next city */
			int min = distance[currentCityIndex][i];

			/* j : possible previous city for the next city */
			for (int j = 0; j < numOfCities; ++j)
			{
				/* if the next city is not same with the previous city,
				   and if previous city is not visited yet */
				if (i != j && visited[j] == false)
				{
					/* get distance from possible previous city to the next city 
					   and get the min distance */
					if (min > distance[j][i])
						min = distance[j][i];
				}
			}
			/* set lower bound as sum of all the possible distance */
			lower_bound += min;
		}
	}

	/* case for the next city is 0 (first base city) */
	int min = INT_MAX;

	/* j : possible previous city before returning to the base city  */
	for (int j = 0; j < numOfCities; ++j)
	{
		/* if the city is not visited yet */
		if (visited[j] == false)
		{
			/* get distance from possible previous city to the base city
			   and get the min distance */
			if (min > distance[j][0])
				min = distance[j][0];
		}
	}
	/* set lower bound as sum of all the possible distance */
	lower_bound += min;
	return lower_bound;
}

void TSP(const MAP& distance, int numOfCities, int depth,
	std::vector<bool>& visited, int currentCityIndex,
	std::vector<int>& solution_so_far, int& cost_solution_so_far,
	std::vector<int>& best_solution_so_far, int& cost_best_solution_so_far)
{
	/* if all cities are visited and only final step (returning to base) is left */
	if (depth == numOfCities)
	{
		/* add the base city and calculate the cost of distance */
		solution_so_far.push_back(0);
		cost_solution_so_far += distance[currentCityIndex][0];

		/* update the best so far */
		if (cost_solution_so_far < cost_best_solution_so_far)
		{
			cost_best_solution_so_far = cost_solution_so_far;
			best_solution_so_far = solution_so_far;
		}

		/* remove the base city and the cost of distance */
		cost_solution_so_far -= distance[currentCityIndex][0];
		solution_so_far.pop_back();
		return;
	}

	/* key - bound, value - cities */
	std::multimap<int, int> ordered_cities;

	/* generate nodes and order them by bound */
	for (int i = 0; i < numOfCities; ++i) 
	{
		/* if the city is not visited yet */
		if (visited[i] == false)
		{
			/* add the next city and calculate the cost of distance */
			visited[i] = true;
			solution_so_far.push_back(i);
			cost_solution_so_far += distance[currentCityIndex][i];

			/* calculate the lower bound */
			int lower_bound = GetLowerBound(distance, numOfCities, visited, i);
			lower_bound += cost_solution_so_far;

			/* insert the city with lower bound (multimap : sort automatically) */
			ordered_cities.insert(std::pair<int, int>(lower_bound, i));

			/* remove the next city and the cost of distance */
			cost_solution_so_far -= distance[currentCityIndex][i];
			solution_so_far.pop_back();
			visited[i] = false;
		}
	}

	/* best first search */
	std::multimap<int, int>::const_iterator b = ordered_cities.begin(),
		e = ordered_cities.end();
	for (; b != e; ++b) 
	{
		/* add the next city and calculate the cost of distance */
		int lower_bound   = b->first;
		int nextCityIndex = b->second;
		visited[nextCityIndex] = true;
		solution_so_far.push_back(nextCityIndex);
		cost_solution_so_far += distance[currentCityIndex][nextCityIndex];
		
		/* branch cancelation check */
		if (lower_bound < cost_best_solution_so_far) 
		{
			TSP(distance, numOfCities, depth + 1, visited, nextCityIndex,
				solution_so_far, cost_solution_so_far,
				best_solution_so_far, cost_best_solution_so_far);
		}

		/* remove the next city and the cost of distance */
		cost_solution_so_far -= distance[currentCityIndex][nextCityIndex];
		solution_so_far.pop_back();
		visited[nextCityIndex] = false;
	}
}

std::vector<int> SolveTSP(char const* filename)
{
	std::ifstream in;
	int TotalCity;

	in.open(filename);
	if (!in) 
	{
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}
	
	/* store the number of city */
	in >> TotalCity;

	/* build the matrix with given distance cost */
	MAP map(TotalCity);
	for (int i = 0; i < TotalCity; ++i)
		map[i] = std::vector<int>(TotalCity, 0);

	for (int i = 0; i < TotalCity; ++i)
	{
		for (int j = i + 1; j < TotalCity; ++j)
		{
			in >> map[i][j];
			map[j][i] = map[i][j];
		}
	}
	in.close();

	std::vector<bool> visited(TotalCity, false);	
	visited[0] = true;
	std::vector<int> solution_so_far;
	solution_so_far.push_back(0); // start with city 0
	int cost_solution_so_far = 0;
	std::vector<int> best_solution_so_far;
	int cost_best_solution_so_far = INT_MAX;
	TSP(map, TotalCity, 1, visited, 0,
		solution_so_far, cost_solution_so_far,
		best_solution_so_far, cost_best_solution_so_far);
	return best_solution_so_far;
}