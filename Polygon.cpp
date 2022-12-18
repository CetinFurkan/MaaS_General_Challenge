#include "Polygon.h"

#include <iostream>

#include "utility.hpp"

Polygon::Polygon()
{

}

Polygon::~Polygon()
{

}    

void Polygon::appendPoint(float x, float y)
{
    Point p;
    p.x = x;
    p.y = y;
    plist.push_back(p);
}

Point Polygon::getPointAt(int i)
{
    return plist.at(i);
}

int Polygon::pointsSize()
{
    return plist.size();
}

// Prints convex hull of a set of n points.
void Polygon::createFromConvexHull(Point points[], int n)
{
	// Find the bottommost point
	float ymin = points[0].y; 
	int min = 0;
	for (int i = 1; i < n; i++)
	{
		float y = points[i].y;

		// Pick the bottom-most or choose the left
		// most point in case of tie
		if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
        {
			ymin = points[i].y;
            min = i;
        }
	}

	// Place the bottom-most point at first position
	ch::swap(points[0], points[min]);

	// Sort n-1 points with respect to the first point.
	// A point p1 comes before p2 in sorted output if p2
	// has larger polar angle (in counterclockwise
	// direction) than p1
	p0 = points[0];
	qsort(&points[1], n-1, sizeof(Point), ch::compare);

	// If two or more points make same angle with p0,
	// Remove all but the one that is farthest from p0
	// Remember that, in above sorting, our criteria was
	// to keep the farthest point at the end when more than
	// one points have same angle.
	int m = 1; // Initialize size of modified array
	for (int i=1; i<n; i++)
	{
		// Keep removing i while angle of i and i+1 is same
		// with respect to p0
		while (i < n-1 && ch::orientation(p0, points[i], points[i+1]) == 0)
			i++;

		points[m] = points[i];
		m++; // Update size of modified array
	}

	// If modified array of points has less than 3 points,
	// convex hull is not possible
	if (m < 3) return;

	// Create an empty stack and push first three points
	// to it.
	stack<Point> S;
	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	// Process remaining n-3 points
	for (int i = 3; i < m; i++)
	{
		// Keep removing top while the angle formed by
		// points next-to-top, top, and points[i] makes
		// a non-left turn
		while (S.size()>1 && ch::orientation(ch::nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
        
		S.push(points[i]);
	}

	// Now stack has the boundry points of the polygon
    // They needed to be kept into plist vector
	plist.clear();
    std::cout << "Polygon made of convex hull of the set: " << std::endl;

	while (!S.empty())
	{
		Point p = S.top();

        std::cout << "(" << p.x << ", " << p.y <<")" << std::endl;
        appendPoint(p.x,p.y);

		S.pop();
	}
}