#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <utility>
#include <float.h>

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

/* function for sorting points set in x ascending order */
bool compareX(Point pt1, Point pt2)
{
	return pt1.x < pt2.x;
}
/* function for sorting points set in y ascending order */
bool compareY(Point pt1, Point pt2)
{
	return pt1.y < pt2.y;
}
/* function to get the shortest distance in the strip area with given offset */
float getClosestDistanceInStrip(std::vector<Point> stripPoints, float stripOffset)
{
	float minDistance = stripOffset;

	/* loop for calculating the min distance in strip area */
	for (unsigned i = 0; i < stripPoints.size(); ++i)
	{
		/* given points set is sorted in y ascending order
		   if the distance of pair of points is greater than min distance,
		   skip farther points*/
		for (unsigned j = i + 1;
			(j < stripPoints.size()) &&
			(stripPoints[j].y - stripPoints[i].y < minDistance);
			++j)
		{
			float distance = std::pow((stripPoints[i].x - stripPoints[j].x), 2)
						   + std::pow((stripPoints[i].y - stripPoints[j].y), 2);
			if (minDistance > distance)
				minDistance = distance;
		}	
	}
	return minDistance; // square distance
}
/* if point set has less or equal to 3 points, calculate the distance */
float getBaseCaseDistance(std::vector<Point> points)
{
	float minDistance = FLT_MAX;

	for (unsigned i = 0; i < points.size(); ++i)
	{
		for (unsigned j = i + 1; j < points.size(); ++j)
		{
			float distance = std::pow((points[i].x - points[j].x), 2)
						   + std::pow((points[i].y - points[j].y), 2);

			if (minDistance > distance)
				minDistance = distance;
		}
	}
	return minDistance; // square distance
}
/* function to split the points on the vertical dividing line, and
   to get the index of given point in sorted point set along x-axis */
int GetxPointIndex(const std::vector<Point>& xPoints, Point yPoint, int middlePointIndex)
{
	/* find an index by checking from the middle point to the left and right */
	int index = middlePointIndex;
	int sign = 1;
	int step = 2; /* step count      : 2 3 4 5 6 7 8 9 ...
				     calculated step : 1 1 2 2 3 3 4 4 ... 
		-> finding index would be in following orders
			 middlePoint, middlePoint +1, middlePoint -1, middlePoint +2, middlePoint -2,
			 middlePoint +3, middlePoint -3, middlePoint +4, middlePoint -4 ... */
	bool outOfBoundary[3] = { false, false, false }; /* variable to check index boundary
									[0] : left boundary, [1] : right boundary,
									[2] : if boundary overflow happens in the current loop*/

	while (true)
	{
		/* check the left boundary */
		if (index < 0)
		{
			outOfBoundary[0] = true;
			outOfBoundary[2] = true;
		}
		/* check the right boundary */
		else if (static_cast<unsigned>(index) >= xPoints.size())
		{
			outOfBoundary[1] = true;
			outOfBoundary[2] = true;
		}
		/* no items found, which could not be happened */
		if (outOfBoundary[0] && outOfBoundary[1])
			exit(EXIT_FAILURE); // error
		/* if boundary overflow happens in the current loop */
		if (outOfBoundary[2])
		{
			/* update the next index and move to the next iteration */
			index = middlePointIndex + sign * (step >> 1);
			++step;
			sign = -sign;
			outOfBoundary[2] = false;
			continue;
		}
		/* if the index is found, then return */
		if (xPoints[index].x == yPoint.x && xPoints[index].y == yPoint.y)
			return index;
		/* update the next index */
		index = middlePointIndex + sign * (step >> 1);
		++step;
		sign = -sign;
	}
}

float closestPair_aux(std::vector<Point> xPoints, std::vector<Point> yPoints, int size)
{
	/* base case */
	if (size <= 3)
		return getBaseCaseDistance(yPoints);

	/* the criteria to partition the two plane */
	int middlePointIndex = size / 2;
	Point middlePoint = { xPoints[middlePointIndex].x,
						  xPoints[middlePointIndex].y };

	/* points set sorted in x ascending order for the left plane */
	std::vector<Point> leftPartitionXPoints(xPoints.begin(),
											xPoints.begin() + middlePointIndex);
	/* points set sorted in x ascending order for the right plane */
	std::vector<Point> rightPartitionXPoints(xPoints.begin() + middlePointIndex,
											 xPoints.end());
	/* points set sorted in y ascending order for the left plane */
	std::vector<Point> leftPartitionYPoints;
	/* points set sorted in y ascending order for the right plane */
	std::vector<Point> rightPartitionYPoints;

	/* divide the points to keep the y sorted points set */
	for (int i = 0; i < size; ++i)
	{
		if (yPoints[i].x < middlePoint.x)
			leftPartitionYPoints.push_back(yPoints[i]); // left plane y sorted
		else if (yPoints[i].x > middlePoint.x)
			rightPartitionYPoints.push_back(yPoints[i]); // right plane y sorted
		/* if the point is on the vertical dividing line, and
		   if the point is considered to be in the left plane of the x sorted points set,
		   put the point into the left, otherwise, right */
		else if (GetxPointIndex(xPoints, yPoints[i], middlePointIndex) < middlePointIndex)
			leftPartitionYPoints.push_back(yPoints[i]);
		else
			rightPartitionYPoints.push_back(yPoints[i]);
	}
	/* recursive function call with the divided plane and points set */
	float leftClosestDistance  = closestPair_aux(leftPartitionXPoints,
								leftPartitionYPoints, middlePointIndex);
	float rightClosestDistance = closestPair_aux(rightPartitionXPoints,
								rightPartitionYPoints, size - middlePointIndex);

	/* get the minimum distance from two planes and set it as the offset of line strip area */
	float stripOffset = std::min(leftClosestDistance, rightClosestDistance);

	/* store the points if it is in the line strip area */
	std::vector<Point> stripPoints;
	for (int i = 0; i < size; ++i)
	{
		if (std::abs(yPoints[i].x - middlePoint.x) < stripOffset)
			stripPoints.push_back(yPoints[i]);
	}
	/* get the minimum distance in the line strip area,
	   if the closest distance of the strip points is greater than the offset,
	   then offset would be returned */
	return getClosestDistanceInStrip(stripPoints, stripOffset);
}

float closestPair ( std::vector< Point > const& points ) {

	/* variable to store the size of the given data set */
	int size = points.size();

	/* exception handling */
	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();

	/* sorted points set along x-axis, and y-axis, respectively */
	std::vector<Point> pointsSortedAlongX(points);
	std::vector<Point> pointsSortedAlongY(points);

	std::sort(pointsSortedAlongX.begin(), pointsSortedAlongX.end(), compareX);
	std::sort(pointsSortedAlongY.begin(), pointsSortedAlongY.end(), compareY);

	/* get the distance of the closest pair */
	return std::sqrt(closestPair_aux(pointsSortedAlongX, pointsSortedAlongY, size));
}