#include "tsp.h"
#include <iostream>
#include <fstream> // ifstream

void TSP(const MAP& distance, int numOfCities, int depth,
	std::vector<bool>& visited, int currentCityIndex,
	std::vector<int>& solution_so_far, int& cost_solution_so_far,
	std::vector<int>& best_solution_so_far, int& cost_best_solution_so_far)
{
	if (depth == numOfCities && distance[currentCityIndex][0] != 0)
	{
		solution_so_far.push_back(0);
		cost_solution_so_far += distance[currentCityIndex][0];
		if (cost_solution_so_far < cost_best_solution_so_far)
		{
			cost_best_solution_so_far = cost_solution_so_far;
			best_solution_so_far = solution_so_far;
		}
		cost_solution_so_far -= distance[currentCityIndex][0];
		solution_so_far.pop_back();
		return;
	}

	for (int i = 0; i < numOfCities; ++i)
	{
		if (visited[i] == false && distance[currentCityIndex][i] != 0)
		{
			visited[i] = true;
			solution_so_far.push_back(i);
			cost_solution_so_far += distance[currentCityIndex][i];
			TSP(distance, numOfCities, depth + 1, visited, i,
				solution_so_far, cost_solution_so_far,
				best_solution_so_far, cost_best_solution_so_far);
			cost_solution_so_far -= distance[currentCityIndex][i];
			solution_so_far.pop_back();
			visited[i] = false;
		}
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
	
	in >> TotalCity;

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