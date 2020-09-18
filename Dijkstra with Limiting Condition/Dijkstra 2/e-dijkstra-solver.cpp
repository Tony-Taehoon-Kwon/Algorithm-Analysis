#include "e-dijkstra-solver.h"
#include <fstream> // ifstream
#include <iostream> // cout
#include <vector>
#include <list>
#include <queue>
#include <limits.h> // INT_MAX

typedef std::priority_queue<std::pair<int, int>,
	std::vector<std::pair<int, int>>> BINARY_HEAP;
typedef std::vector<std::list<std::pair<int, int>>> EDGES;

bool IsReachableToAllVertices(BINARY_HEAP& BH, int numOfVertices,
	int maxRecharges, int range, const EDGES& edges, int src)
{
	std::vector<bool> visited(numOfVertices, false);
	std::vector<bool> evaluated(numOfVertices, false);
	std::vector<int> r(numOfVertices, 0);
	std::vector<int> k(numOfVertices, maxRecharges);
	std::vector<int> calculated_relaxation(numOfVertices, 0);

	/* vehicle starts empty, so need to recharge before departing */
	r[src] = range;
	--k[src];

	/* starts from src and set visited for source vertex */
	calculated_relaxation[src] = r[src] + k[src] * range;
	BH.push(std::pair<int, int>(calculated_relaxation[src], src));
	visited[src] = true;

	int currFuel, u, v, weight, prev_r, prev_k;
	while (!BH.empty())
	{
		u = BH.top().second;
		BH.pop();

		/* skip if u was already checked */
		if (evaluated[u])
			continue;

		evaluated[u] = true;

		/* for each neighbor v of u */
		for (auto it = edges[u].begin(); it != edges[u].end(); ++it)
		{
			v = it->first;
			weight = it->second;

			/* skip if v was already checked */
			if (evaluated[v])
				continue;

			/* if path is out of range */
			if (weight > range)
				continue;
			
			/* keep the old information */
			prev_r = r[v];
			prev_k = k[v];

			/* new information to be calculated */
			r[v] = r[u];
			k[v] = k[u];

			/* if need to recharge */
			if (weight > r[v])
			{
				/* if recharge count is left */
				if (k[v] > 0)
				{
					r[v] = range;
					k[v]--;
				}
				else
				{
					r[v] = prev_r;
					k[v] = prev_k;
					continue;
				}
			}

			/* update range and calculate relaxation */
			r[v] -= weight;
			currFuel = r[v] + range * k[v];

			/* if it is not visited before or it's better relaxation than old */
			if (!visited[v] || currFuel > calculated_relaxation[v])
			{
				visited[v] = true;
				BH.push(std::pair<int, int>(currFuel, v));
				calculated_relaxation[v] = currFuel;
			}
			else
			{
				r[v] = prev_r;
				k[v] = prev_k;
			}
		}
	}

	/* if there is no path from one node to another, return false
	   otherwise, return true */
	for (int i = 0; i < numOfVertices; ++i)
	{
		if (!visited[i])
		{
			//std::cout << i << std::endl;
			return false;
		}
	}
	return true;
}

int e_dijkstra_solver(char const* filename)
{
	int r_min = INT_MAX;

	std::ifstream in;
	in.open(filename);
	if (!in)
	{
		std::cout << "Unable to open file";
		exit(1); // terminate with error;
	}

	/* store the first row of the input */
	int numOfVertices, maxRecharges, numOfEdges;
	in >> numOfVertices;
	in >> maxRecharges;
	in >> numOfEdges;

	/* list of edge with attribute of <destination vertex index, weight>
	   respect to source vertex index */
	EDGES edges(numOfVertices);
	
	for (int i = 0; i < numOfEdges; ++i)
	{
		int src, dest, weight;
		in >> src;
		in >> dest;
		in >> weight;

		if (r_min > weight)
			r_min = weight;

		edges[src].push_back(std::pair<int, int>(dest, weight));
		edges[dest].push_back(std::pair<int, int>(src, weight));
	}

	in.close();

	r_min = r_min * maxRecharges;
	int r_max = r_min * numOfVertices;
	
	/* Binary heap storing the following data :
	   remain_fuel = remain_charge + remain_recharge * range
	   <remain_fuel, vertex_index> */
	BINARY_HEAP BH;

	while (r_max - r_min > 1)
	{
		int middle = (r_min + r_max) / 2;

		bool result = true;

		/* test the dijkstra */
		for (int i = 0; i < numOfVertices; ++i)
		{
			if (!IsReachableToAllVertices(BH, numOfVertices, maxRecharges,
				middle, edges, i))
			{
				result = false;
				break;
			}
		}

		if (result)
			r_max = middle;
		else
			r_min = middle;
	}
	return r_max;
}